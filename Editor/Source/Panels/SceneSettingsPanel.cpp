#include "SceneSettingsPanel.h"
#include "Core/Engine.h"
#include "Core/Logger.h"
#include "Core/Memory/CeMemory.h"
#include "Math/Math.h"
#include "Renderer/Camera/CameraSystem.h"
#include "Renderer/Camera/PerspectiveCamera.h"
#include "CoreEditorUtils.h"
#include "Utils/StringUtils.h"

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
        CeMemory::Zero(&sceneNameBuffer, 256);

        if (scene)
            CeMemory::Copy(&sceneNameBuffer, scene->GetName().c_str(), scene->GetName().length());
    }

    void SceneSettingsPanel::OnImGuiRender()
    {
        if (!scene || scene == nullptr)
            return;

        ImGui::Begin("Scene Settings");

        if (ImGui::InputText("Name", sceneNameBuffer, 256))
            scene->SetName(std::string(sceneNameBuffer));

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

                float directionArray[3] = {Math::RadToDeg(environment->directionalLight->GetDirection()->x), Math::RadToDeg(environment->directionalLight->GetDirection()->y), Math::RadToDeg(environment->directionalLight->GetDirection()->z)};
                if (ImGui::DragFloat3("Direction", directionArray, 0.1))
                {
                    environment->directionalLight->GetDirection()->Set(Math::DegToRad(directionArray[0]), Math::DegToRad(directionArray[1]), Math::DegToRad(directionArray[2]));
                }

                // EditorUtils::ImGuiVec3Edit("Sepcular", environment->directionalLight.GetSpe);

                ImGui::TreePop();
            }

            bool closeSky = ImGui::TreeNodeEx("Sky");
            Sky *sky = environment->sky;

            if (closeSky)
            {
                // WIP: Sky select string
                const int skyTypeCount = 3;
                const char *skyTypeSelection[skyTypeCount] = {"Color", "Cube Map", "Shader"};
                const char *skyCurrent = skyTypeSelection[(int)sky->GetMode()];

                if (ImGui::BeginCombo("Sky Mode", skyCurrent))
                {
                    for (int i = 0; i < skyTypeCount; i++)
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
                else if (sky->GetMode() == SkyMode::Shader)
                {
                    if (ImGui::Button("Drop Shader File (.vs.glsl || .fs.glsl)"))
                    {
                        // TODO: Display a shader info.
                    }

                    if (ImGui::BeginDragDropTarget())
                    {
                        const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CE_CONTENT_PANEL");

                        if (payload)
                        {
                            const char *name = (const char *)payload->Data;
                            std::string ext = StringUtils::GetFileExtension(name);
                            if (ext == "glsl")
                            {
                                sky->SetShaderName(StringUtils::RemoveFileExtension(StringUtils::RemoveFileExtension(name)));
                            }
                        }

                        ImGui::EndDragDropTarget();
                    }

                    ImGui::Separator();

                    // Draw the shader data
                    auto shaderData = sky->GetSkyShaderData();
                    for (auto data : shaderData)
                    {
                        EditorUtils::RenderDataSet(data, sky);
                    }

                    if (ImGui::Button("Add"))
                    {
                        sky->AddShaderData(sizeof(Vector2), new Vector2(), Data::DataVec2, "Value");
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