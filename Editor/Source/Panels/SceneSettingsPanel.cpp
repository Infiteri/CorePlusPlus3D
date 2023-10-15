#include "SceneSettingsPanel.h"

#include "Core/Engine.h"
#include "Core/Logger.h"
#include "Core/Memory/CeMemory.h"

#include "Math/Math.h"

#include "Renderer/Camera/CameraSystem.h"
#include "Renderer/Camera/PerspectiveCamera.h"

#include "CoreEditorUtils.h"
#include "Utils/StringUtils.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Core
{
    static bool renderCreateCameraDisplay = false;
    static char createCameraNameBuffer[256];
    static char sceneNameBuffer[256];

    SceneSettingsPanel::SceneSettingsPanel()
    {
        this->scene = nullptr;

        CeMemory::Zero(&sceneNameBuffer, 256);
    }

    SceneSettingsPanel::~SceneSettingsPanel()
    {
    }

    void SceneSettingsPanel::UpdateSceneToWorldActive()
    {
        scene = World::GetActive();
        CeMemory::Copy(&sceneNameBuffer, scene->GetName().c_str(), scene->GetName().size() * sizeof(char));
    }

    void SceneSettingsPanel::OnImGuiRender()
    {
        if (!scene || scene == nullptr)
            return;

        ImGui::Begin("Scene Settings");

        if (ImGui::InputText("Name", sceneNameBuffer, 256))
            scene->SetName(std::string(sceneNameBuffer));

        // Camera work
        PerspectiveCamera *sceneCamera = nullptr;
        if (!scene->GetSceneCameraName().empty())
            sceneCamera = CameraSystem::Get(scene->GetSceneCameraName());

        if (sceneCamera != nullptr)
        {
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_AllowItemOverlap;
            bool cameraClosed = ImGui::TreeNodeEx("camera", flags, scene->GetSceneCameraName().c_str());

            if (cameraClosed)
            {
                float fov = Math::RadToDeg(sceneCamera->GetFOV());
                float near = sceneCamera->GetNear();
                float far = sceneCamera->GetFar();

                if (ImGui::DragFloat("FOV", &fov, 1.0f, 1.0f, 179.0f))
                {
                    sceneCamera->SetFOV(Math::DegToRad(fov));
                    sceneCamera->UpdateProjection();
                }

                if (ImGui::DragFloat("Near", &near, 0.1f, 0.01f))
                {
                    sceneCamera->SetNear(near);
                    sceneCamera->UpdateProjection();
                }

                if (ImGui::DragFloat("Far", &far, 0.1f, 1.0f))
                {
                    sceneCamera->SetFar(far);
                    sceneCamera->UpdateProjection();
                }

                ImGui::TreePop();
            }
        }
        else
        {
            if (renderCreateCameraDisplay)
            {
                ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_Always);

                ImGui::Begin("Create Camera", &renderCreateCameraDisplay, ImGuiWindowFlags_NoResize);

                ImGui::Text("Camera Settings");

                static float fov = 90.0f;       // Default FOV value
                static float nearClip = 0.1f;   // Default Near value
                static float farClip = 1000.0f; // Default Far value

                ImGui::InputText("Name", createCameraNameBuffer, 256);
                ImGui::SliderFloat("FOV", &fov, 1.0f, 179.0f);
                ImGui::SliderFloat("Near", &nearClip, 0.01f, 100.0f);
                ImGui::DragFloat("Far", &farClip, 1.0f, 0.001f);

                if (ImGui::Button("Done"))
                {
                    CameraSystem::Generate(createCameraNameBuffer, Math::DegToRad(fov), Engine::GetWindowAspect(), nearClip, farClip);
                    scene->SetSceneCameraName(createCameraNameBuffer);
                    renderCreateCameraDisplay = false;
                    CeMemory::Zero(&createCameraNameBuffer, 256);
                }

                ImGui::End();
            }

            if (ImGui::Button("Generate Scene Camera"))
            {
                renderCreateCameraDisplay = true;
            }
        }

        // -- Environment --
        SceneEnvironment *environment = scene->GetEnvironment();

        // Light
        bool closeEnv = ImGui::TreeNodeEx("Environment");

        if (closeEnv)
        {
            bool closeDir = ImGui::TreeNodeEx("Directional Light");

            if (closeDir)
            {

                Color *color = environment->directionalLight->GetColor();
                float directionalLightColors[4] = {color->r / 255, color->g / 255, color->b / 255, color->a / 255};

                if (ImGui::ColorEdit4("Color", directionalLightColors))
                {
                    color->Set(directionalLightColors[0] * 255, directionalLightColors[1] * 255, directionalLightColors[1] * 255, directionalLightColors[3] * 255);
                }

                float directionArray[3] = {environment->directionalLight->GetDirection()->x, environment->directionalLight->GetDirection()->y, environment->directionalLight->GetDirection()->z};
                if (ImGui::DragFloat3("Direction", directionArray, 0.1))
                {
                    environment->directionalLight->GetDirection()->Set(directionArray[0], directionArray[1], directionArray[2]);
                }

                ImGui::TreePop();
            }

            bool closeSky = ImGui::TreeNodeEx("Sky");
            Sky *sky = environment->sky;

            if (closeSky)
            {
                // WIP: Sky select string
                const char *skyTypeSelection[2] = {"Color", "Cube Map"};
                const char *skyCurrent = skyTypeSelection[(int)sky->GetMode()];

                if (ImGui::BeginCombo("Sky Mode", skyCurrent))
                {
                    for (int i = 0; i < 2; i++)
                    {
                        bool isSelected = (skyCurrent == skyTypeSelection[i]);

                        if (ImGui::Selectable(skyTypeSelection[i], isSelected))
                        {
                            skyCurrent = skyTypeSelection[i];
                            sky->SetMode((SkyMode)i);
                        }

                        if (isSelected)
                            ImGui::SetItemDefaultFocus();
                    }

                    ImGui::EndCombo();
                }

                // Actual props editor
                if (sky->GetMode() == SkyMode::Color)
                {
                    Color *col = sky->GetColor();
                    float colors[4] = {col->r / 255, col->g / 255, col->b / 255, col->a / 255};

                    if (ImGui::ColorEdit4("Color", colors))
                        col->Set(colors[0] * 255, colors[1] * 255, colors[2] * 255, colors[3] * 255);
                }
                else if (sky->GetMode() == SkyMode::CubeMap)
                {
                    if (ImGui::Button("Cube Map"))
                    {
                        // TODO: Display a nice cube map viewer.
                    }

                    if (ImGui::BeginDragDropTarget())
                    {
                        const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CE_CONTENT_PANEL");

                        if (payload)
                        {
                            const char *name = (const char *)payload->Data;
                            std::string ext = StringUtils::GetFileExtension(name);
                            if (ext == "ce_cubemap")
                            {
                                sky->CreateCubeTexture(name);
                            }
                        }

                        ImGui::EndDragDropTarget();
                    }
                }

                ImGui::TreePop();
            }

            ImGui::TreePop();
        }
        // -----------------

        ImGui::End();
    }
}