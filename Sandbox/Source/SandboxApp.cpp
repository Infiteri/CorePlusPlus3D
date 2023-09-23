#include "Core.h"
#include "Core/Launch/Launch.h"

#include "SandboxUserData.h"
#include "SandboxLayer.h"

#include <imgui.h>

static SandboxUserData *MyData;

class Sandbox : public Core::Application
{
public:
    Sandbox(){};

    void Init()
    {
        SandboxUserData data;
        data.lastCameraFov = 90;
        Core::Engine::GenerateUserData(&data, sizeof(SandboxUserData));
        MyData = (SandboxUserData *)Core::Engine::GetUserData();

        Core::Renderer::SetBackgroundColor(0, 0, 0.1 * 255, 255);
        Core::LayerStack::PushLayer(new SandboxLayer());
        Core::MaterialManager::Load("EngineResources/Materials/Default.ce_mat");

        MyData->scene = new Core::Scene();
        MyData->scene->Init();
        MyData->scene->Start();

        MyData->scene->GenerateAndActivateSceneCamera("Scene1Camera", Core::Math::DegToRad(data.lastCameraFov), Core::Engine::GetWindowAspect(), 0.01f, 1000.0f);

        Core::Actor *a = new Core::Actor();
        MyData->scene->AddActor(a);
        auto mesh = a->AddComponent<Core::MeshComponent>();
        mesh->SetGeometry(new Core::BoxGeometry(1, 1, 1));

        Core::CeMemory::TracePrintSize("sizeof(SandboxUserData)", sizeof(SandboxUserData));
    };

    void Render()
    {
        MyData->scene->Render();
    }

    void Shutdown()
    {
        MyData->scene->Stop();

        Core::CeMemory::Free(MyData);
    };
};

Core::Application *Core::CreateApplication(Core::EngineConfiguration *config)
{
    return new Sandbox();
};