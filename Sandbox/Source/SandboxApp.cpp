#include "Core.h"
#include "Core/Launch/Launch.h"

#include <imgui.h>

static Core::Scene *scene;

class SandboxLayer : public Core::Layer
{
public:
    SandboxLayer(){};
    ~SandboxLayer(){};

    void OnImGuiRender(){

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

        Core::Actor *a = new Core::Actor();
        scene->AddActor(a);
        auto mesh = a->AddComponent<Core::MeshComponent>();
        mesh->SetGeometry(new Core::BoxGeometry(1, 1, 1));

        Core::CameraSystem::Generate("ActiveCamera", Core::Math::DegToRad(90), Core::Engine::GetWindowAspect(), 0.01, 1000);
        Core::CameraSystem::Activate("ActiveCamera");
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