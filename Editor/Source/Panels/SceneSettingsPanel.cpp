#include "SceneSettingsPanel.h"

#include "Core/Engine.h"
#include "Core/Memory/CeMemory.h"

#include "Math/Math.h"

#include "Renderer/Camera/CameraSystem.h"
#include "Renderer/Camera/PerspectiveCamera.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Core
{
    static bool renderCreateCameraDisplay = false;
    SceneSettingsPanel::SceneSettingsPanel()
    {
        this->scene = nullptr;
    }

    SceneSettingsPanel::~SceneSettingsPanel()
    {
    }

    void SceneSettingsPanel::OnImGuiRender()
    {
        if (!scene || scene == nullptr)
            return;

        ImGui::Begin("Scene Settings");

        // Camera work
        PerspectiveCamera *sceneCamera = CameraSystem::Get(scene->GetSceneCameraName());
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

                ImGui::TreePop(); // Close the tree node when done
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

                static char buffer[256];
                CeMemory::Zero(buffer, 256);

                ImGui::InputText("Name", buffer, 256);
                ImGui::SliderFloat("FOV", &fov, 1.0f, 179.0f);
                ImGui::SliderFloat("Near", &nearClip, 0.1f, 100.0f);
                ImGui::DragFloat("Far", &farClip, 1.0f, 0.01f);

                if (ImGui::Button("Done"))
                {
                    CameraSystem::Generate(buffer, Math::DegToRad(fov), Engine::GetWindowAspect(), nearClip, farClip);
                    scene->SetSceneCameraName(buffer);

                    renderCreateCameraDisplay = false;
                }

                ImGui::End();
            }

            if (ImGui::Button("Generate Scene Camera"))
            {
                renderCreateCameraDisplay = true;
            }
        }

        ImGui::End();
    }
}