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
        //? ---------------- User -----------
        SandboxUserData data;
        data.lastCameraFov = 90;
        Core::Engine::GenerateUserData(&data, sizeof(SandboxUserData));
        MyData = (SandboxUserData *)Core::Engine::GetUserData();
        //? ---------------------------------

        Core::Renderer::SetBackgroundColor(0, 0, 0.1 * 255, 255);
        Core::LayerStack::PushLayer(new SandboxLayer());
        Core::MaterialManager::Load("EngineResources/Materials/Default.ce_mat");

        // World Setup
        Core::World::Create("CoolScene");
        Core::World::Activate("CoolScene");

        Core::World::GetActive()->GenerateAndActivateSceneCamera("Scene1Camera", Core::Math::DegToRad(data.lastCameraFov), Core::Engine::GetWindowAspect(), 0.01f, 1000.0f);

        Core::Actor *a = new Core::Actor();
        Core::World::GetActive()->AddActor(a);
        auto mesh = a->AddComponent<Core::MeshComponent>();
        mesh->SetGeometry(new Core::BoxGeometry(1, 1, 1));
        mesh->SetMaterialFromName("Material");
        // End World Setup

        Core::CeMemory::TracePrintSize("sizeof(SandboxUserData)", sizeof(SandboxUserData));
    };

    void Render()
    {
    }

    void Shutdown()
    {
        Core::CeMemory::Free(MyData);
    };
};

Core::Application *Core::CreateApplication(Core::EngineConfiguration *config)
{
    return new Sandbox();
};