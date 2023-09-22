#include "Core.h"
#include "Core/Launch/Launch.h"

#include <imgui.h>

static Core::DepthMode lastMode;

static int lastWidth = 1;
static int lastHeight = 1;
static int lastDepth = 1;

static float lastX = 1;
static float lastY = 1;
static float lastZ = 1;

static Core::Mesh *mesh;

class SandboxLayer : public Core::Layer
{
public:
    SandboxLayer(){};
    ~SandboxLayer(){};

    void OnImGuiRender()
    {
        ImGui::Begin("Hello");

        if (ImGui::DragInt("Width", &lastWidth, 0.1f, 1) || ImGui::DragInt("Height", &lastHeight, 0.1f, 1) || ImGui::DragInt("Depth", &lastDepth), 0.1f, 1)
        {
            // mesh->SetGeometry(new Core::BoxGeometry(lastWidth, lastHeight, lastDepth));
        }

        float pos[3] = {mesh->GetTransform()->GetPosition()->x, mesh->GetTransform()->GetPosition()->y, mesh->GetTransform()->GetPosition()->z};

        if (ImGui::DragFloat3("Position", pos))
        {
            mesh->GetTransform()->GetPosition()->x = pos[0];
            mesh->GetTransform()->GetPosition()->y = pos[1];
            mesh->GetTransform()->GetPosition()->z = pos[2];
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
        Core::Renderer::SetBackgroundColor(255, 255, 255, 255);
        Core::LayerStack::PushLayer(new SandboxLayer());

        // Core::MaterialConfiguration config;
        // config.color.Set(0, 125, 255, 255);
        // config.name = "Material";
        // config.colorTextureName = "EngineResources/Images/crate.png";

        Core::MaterialManager::Load("EngineResources/Materials/Default.ce_mat");

        mesh = new Core::Mesh();
        mesh->SetMaterialFromName("Material");
        mesh->SetGeometry(new Core::BoxGeometry(lastWidth, lastHeight, lastDepth));
        mesh->Init();

        Core::CameraSystem::Generate("ActiveCamera", Core::Math::DegToRad(90), Core::Engine::GetWindowAspect(), 0.01, 1000);
        Core::CameraSystem::Activate("ActiveCamera");
    };

    void Render()
    {
        mesh->Render();
    }

    void Shutdown()
    {
        delete mesh;
    };
};

Core::Application *Core::CreateApplication(Core::EngineConfiguration *config)
{
    return new Sandbox();
};