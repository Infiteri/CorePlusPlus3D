#include "Core.h"
#include "Core/Launch/Launch.h"

#include <imgui.h>

static Core::DepthMode lastMode;

static int lastWidth = 1;
static int lastHeight = 1;
static int lastDepth = 1;
static Core::Mesh *mesh;

class SandboxLayer : public Core::Layer
{
public:
    SandboxLayer(){};
    ~SandboxLayer(){};

    void OnImGuiRender()
    {
        ImGui::Begin("Hello");

        // List of depth modes
        static const char *depthModes[] = {"Less", "Lequal", "Always"};
        static int selectedDepthMode = static_cast<int>(lastMode); // Initialize with the current mode

        // Create the dropdown menu
        if (ImGui::BeginCombo("Depth Mode", depthModes[selectedDepthMode]))
        {
            for (int i = 0; i < IM_ARRAYSIZE(depthModes); i++)
            {
                bool isSelected = (selectedDepthMode == i);
                if (ImGui::Selectable(depthModes[i], isSelected))
                {
                    selectedDepthMode = i;

                    // Update the 'lastMode' enum based on the selected index
                    switch (selectedDepthMode)
                    {
                    case 0:
                        Core::Renderer::SetDepthMode(Core::DepthMode::Less);
                        break;
                    case 1:
                        Core::Renderer::SetDepthMode(Core::DepthMode::Lequal);
                        break;
                    case 2:
                        Core::Renderer::SetDepthMode(Core::DepthMode::Always);
                        break;
                        // Add more cases if you have additional depth modes
                    }
                }

                if (isSelected)
                    ImGui::SetItemDefaultFocus(); // Set the default selection
            }

            ImGui::EndCombo();
        }

        if (ImGui::DragInt("Width", &lastWidth, 0.1f, 1) || ImGui::DragInt("Height", &lastHeight, 0.1f, 1) || ImGui::DragInt("Depth", &lastDepth), 0.1f, 1)
        {
            mesh->SetGeometry(new Core::BoxGeometry(lastWidth, lastHeight, lastDepth));
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
        Core::Renderer::SetBackgroundColor(0, 0, 0, 255);
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