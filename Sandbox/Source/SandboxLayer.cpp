#include "SandboxLayer.h"
#include "SandboxUserData.h"

#include <imgui.h>

void SandboxLayer::OnImGuiRender()
{
    ImGui::Begin("Camera");

    // Testing for camera values
    SandboxUserData *MyData = (SandboxUserData *)Core::Engine::GetUserData();
    if (ImGui::DragFloat("FOV", &MyData->lastCameraFov))
    {
        auto camera = Core::CameraSystem::Get(Core::World::GetActive()->GetSceneCameraName());

        if (camera != nullptr)
        {
            camera->SetFOV(Core::Math::DegToRad(MyData->lastCameraFov));
            camera->UpdateProjection();
        }
    }

    ImGui::End();
};
