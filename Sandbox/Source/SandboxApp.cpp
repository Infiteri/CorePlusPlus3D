#include "Core.h"
#include "Core/Launch/Launch.h"

#include <imgui.h>

static Core::Scene *scene;

static float lastFov = 90;

class SandboxLayer : public Core::Layer
{
public:
    SandboxLayer(){};
    ~SandboxLayer(){};

    void OnImGuiRender()
    {
        ImGui::Begin("Camera");

        // Testing for camera values
        if (ImGui::DragFloat("FOV", &lastFov))
        {
            auto camera = Core::CameraSystem::Get(scene->GetSceneCameraName());

            if (camera != nullptr)
            {
                camera->SetFOV(Core::Math::DegToRad(lastFov));
                camera->UpdateProjection();
            }
        }

        ImGui::End();
    };
};

class Sandbox : public Core::Application
{
public:
    Sandbox(){};

    void Init()
    {
        Core::Renderer::SetBackgroundColor(0, 0, 0.1 * 255, 255);
        Core::LayerStack::PushLayer(new SandboxLayer());

        Core::MaterialManager::Load("EngineResources/Materials/Default.ce_mat");

        scene = new Core::Scene();  
        scene->Init();
        scene->Start();

        scene->GenerateAndActivateSceneCamera("Scene1Camera", Core::Math::DegToRad(lastFov), Core::Engine::GetWindowAspect(), 0.01f, 1000.0f);

        Core::Actor *a = new Core::Actor();
        scene->AddActor(a);
        auto mesh = a->AddComponent<Core::MeshComponent>();
        mesh->SetGeometry(new Core::BoxGeometry(1, 1, 1));
    };

    void Render()
    {
        scene->Render();
    }

    void Shutdown()
    {
        scene->Stop();

        delete scene;
    };
};

Core::Application *Core::CreateApplication(Core::EngineConfiguration *config)
{
    return new Sandbox();
};