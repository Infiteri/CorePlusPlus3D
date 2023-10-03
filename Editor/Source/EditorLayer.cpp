#include "EditorLayer.h"

#include "Core.h"

namespace Core
{
    // Note: Switch this to true to enable dockspace
    static bool dockspaceOpen = true;
    static bool opt_fullscreen_persistant = true;
    static bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    static ImVec2 lastFrameViewportSize;

    void EditorLayer::OnAttach()
    {
        // Create editor camera
        CameraSystem::Generate("__EditorCamera__", Math::DegToRad(90), Engine::GetWindowAspect(), 0.01f, 1000.0f);
        CameraSystem::Activate("__EditorCamera__");

        // TODO: From file

        Renderer::SetBackgroundColor(0, 0, 0.1 * 255, 255);

        // World Setup
        World::Create("NewScene");
        World::Activate("NewScene");

        World::GetActive()->GenerateSceneCamera("GameCamera", Math::DegToRad(90), Engine::GetWindowAspect(), 0.01f, 1000.0f);

        {
            Actor *a = new Actor();
            World::GetActive()->AddActor(a);
            auto mesh = a->AddComponent<MeshComponent>();
            mesh->SetGeometry(new BoxGeometry(1, 1, 1));
            mesh->mesh->GetMaterial()->GetColor()->Set(0, 0, 255, 255);
        }
        {
            Actor *a = new Actor();
            a->SetName("A2");
            a->GetTransform()->GetPosition()->Set(-3, 0, 0);

            World::GetActive()->AddActor(a);
            auto mesh = a->AddComponent<MeshComponent>();
            mesh->SetGeometry(new BoxGeometry(1, 1, 1));
            mesh->mesh->GetMaterial()->GetColor()->Set(0, 125, 255, 255);
            mesh->mesh->GetMaterial()->GetColorTexture()->Load("EngineResources/Images/crate.png");
        }
        // TODO: End from file

        sceneHierarchyPanel.UpdateContextToWorldActive();
        sceneSettingsPanel.UpdateSceneToWorldActive();

        CameraSystem::Activate("__EditorCamera__");
    }

    void EditorLayer::OnRender()
    {
    }

    void EditorLayer::OnUpdate()
    {
    }

    void EditorLayer::OnImGuiRender()
    {
        BeginDockspace();

        sceneHierarchyPanel.OnImGuiRender();
        sceneSettingsPanel.OnImGuiRender();

        ImGui::Begin("Test");
        if (ImGui::Button("A"))
        {
            CameraSystem::Activate("__EditorCamera__");
            Renderer::Resize(lastFrameViewportSize.x, lastFrameViewportSize.y);
        }

        if (ImGui::Button("B"))
        {
            if (World::GetActive()->GetSceneCameraName().empty() || World::GetActive()->GetSceneCameraName() == "__NONE__INVALID__")
                CameraSystem::SetActiveCameraToNone();
            else
            {
                CameraSystem::Activate(World::GetActive()->GetSceneCameraName().c_str());
                Renderer::Resize(lastFrameViewportSize.x, lastFrameViewportSize.y);
            }
        }

        if (ImGui::Button("Restart Runtime"))
        {
            World::StopActive();
            World::StartActive();
        }

        ImGui::End();

        RenderSceneViewport();

        EndDockspace();
    }

    void EditorLayer::OnDetach()
    {
    }

    void EditorLayer::BeginDockspace()
    {
        if (opt_fullscreen)
        {
            ImGuiViewport *viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
        ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // DockSpace
        ImGuiIO &io = ImGui::GetIO();
        ImGuiStyle &style = ImGui::GetStyle();
        float minWinSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x = 250.0f;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        style.WindowMinSize.x = minWinSizeX;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
        ImGui::PopStyleVar();
    }

    void EditorLayer::EndDockspace()
    {
        ImGui::End();
    }

    void EditorLayer::RenderSceneViewport()
    {
        ImGui::Begin("Viewport");

        // Update renderer viewport
        ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        if (viewportSize.x != lastFrameViewportSize.x || viewportSize.y != lastFrameViewportSize.y)
        {
            lastFrameViewportSize = viewportSize;
            Renderer::Resize(viewportSize.x, viewportSize.y);
        }
        // End update renderer viewport
        ImGui::Image((void *)(CeU64)(CeU32)(Renderer::GetFrameBuffer()->GetRenderPass(0)->id), viewportSize, ImVec2{0, 1}, ImVec2{1, 0});

        ImGui::End();
    }
}