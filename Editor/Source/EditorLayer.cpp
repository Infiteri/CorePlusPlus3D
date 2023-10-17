#include "EditorLayer.h"

#include "Core.h"

#include <ImGuizmo.h>

namespace Core
{
    // Note: Switch this to true to enable dockspace
    static bool dockspaceOpen = true;
    static bool opt_fullscreen_persistant = true;
    static bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    static ImVec2 lastFrameViewportSize;

    static Scene *EditorScene;

    static Texture *IconPlayTexture;
    static Texture *IconStopTexture;
    static ImGuizmo::OPERATION operation = ImGuizmo::TRANSLATE;
    static PerspectiveMovement *movement;

    void EditorLayer::OnAttach()
    {
        // Create editor camera
        CameraSystem::Generate("__EditorCamera__", Math::DegToRad(90), Engine::GetWindowAspect(), 0.01f, 1000.0f);
        CameraSystem::Activate("__EditorCamera__");

        SceneSerializer ser = SceneSerializer{World::GetActive()};
        New();
        World::InitActive();

        IconPlayTexture = new Texture();
        IconPlayTexture->Load("EngineResources/CeImage/Icons/PlayButton.ce_image");

        IconStopTexture = new Texture();
        IconStopTexture->Load("EngineResources/CeImage/Icons/StopButton.ce_image");

        SetContexts();
        contentBrowserPanel.LoadAssets();

        movement = new PerspectiveMovement();

        currentSceneState = SceneStateStop;
        StopSceneRuntime();
    }

    void EditorLayer::OnImGuiRender()
    {
        BeginDockspace();

        sceneHierarchyPanel.OnImGuiRender();
        sceneSettingsPanel.OnImGuiRender();
        contentBrowserPanel.OnImGuiRender();

        UI_DrawMainTopBar();
        UI_DrawTopPlayStopBar();
        RenderSceneViewport();

        EndDockspace();

        if (Input::GetKey(Keys::R))
        {
            operation = ImGuizmo::ROTATE;
        }

        if (Input::GetKey(Keys::T))
        {
            operation = ImGuizmo::TRANSLATE;
        }

        if (Input::GetKey(Keys::E))
        {
            operation = ImGuizmo::SCALE;
        }
    }

    void EditorLayer::OnDetach()
    {
        delete IconPlayTexture;
        delete IconStopTexture;
    }

    void EditorLayer::OnUpdate()
    {
        if (currentSceneState == SceneStateStop)
            OnUpdateEditor();
        else if (currentSceneState == SceneStatePlay)
            OnUpdateRuntime();
    }

    void EditorLayer::OnEvent(Event *event)
    {
        if (event->GetType() == EventType::WindowResize)
            Renderer::Resize(lastFrameViewportSize.x, lastFrameViewportSize.y);
    }

    void EditorLayer::OnUpdateRuntime()
    {
        sceneHierarchyPanel.selectionContext = nullptr;
    }

    void EditorLayer::OnUpdateEditor()
    {
        movement->Update(CameraSystem::GetActive());
    }

    void EditorLayer::UI_DrawTopPlayStopBar()
    {
        ImGui::Begin("##topbar");

        Texture *tex = currentSceneState == SceneStatePlay ? IconStopTexture : IconPlayTexture;
        if (ImGui::ImageButton((ImTextureID)(CeU64)(CeU32)tex->GetID(), {12, 12}))
        {
            if (currentSceneState == SceneStatePlay)
                StopSceneRuntime();
            else
                StartSceneRuntime();
        }

        ImGui::End();
    }

    void EditorLayer::New()
    {
        World::StopActive();
        // TODO: Create scene window prompt with basic information.
        World::Create("NewSceneCreate");
        World::Activate("NewSceneCreate");
        World::InitActive();

        SetContexts();
    }

    void EditorLayer::Open()
    {
        std::string name = Platform::OpenFileDialog("Core Scene (*.ce_scene)\0*.ce_scene\0");
        if (!name.empty())
            OpenScene(name);
    }

    void EditorLayer::SaveAs()
    {
        std::string name = Platform::SaveFileDialog("Core Scene (*.ce_scene)\0*.ce_scene\0");
        if (!name.empty())
        {
            SceneSerializer ser{World::GetActive()};
            ser.Serialize(name);
            SetContexts();
        }
    }

    void EditorLayer::StartSceneRuntime()
    {
        currentSceneState = SceneStatePlay;
        EditorScene = World::GetActive() ? Scene::GetCopyOfScene(World::GetActive()) : nullptr;

        if (World::GetActive())
            World::GetActive()->ActivateSceneCamera();

        World::StartActive();
    }

    void EditorLayer::StopSceneRuntime()
    {
        currentSceneState = SceneStateStop;
        World::StopActive();

        if (EditorScene)
        {
            World::CopyToActive(EditorScene);
            EditorScene->Destroy();
            delete EditorScene;
            SetContexts();
        }

        CameraSystem::Activate("__EditorCamera__");

        World::InitActive();
    }

    void EditorLayer::UI_DrawMainTopBar()
    {

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::MenuItem("File"))
                ImGui::OpenPopup("FilePopup");

            if (ImGui::BeginPopup("FilePopup"))
            {
                if (ImGui::MenuItem("New", "Ctrl+N"))
                    New();

                if (ImGui::MenuItem("Open...", "Ctrl+O"))
                    Open();

                if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
                    SaveAs();

                ImGui::EndPopup();
            }

            ImGui::EndMainMenuBar();
        }
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

        if (ImGui::BeginDragDropTarget())
        {
            const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CE_CONTENT_PANEL");

            if (payload)
            {
                const char *name = (const char *)payload->Data;
                if (name)
                    if (StringUtils::GetFileExtension(name).compare("ce_scene") == 0)
                        OpenScene(name);
            }

            ImGui::EndDragDropTarget();
        }

        Actor *actorContext = sceneHierarchyPanel.selectionContext;
        PerspectiveCamera *camera = CameraSystem::GetActive();
        if (actorContext != nullptr && camera != nullptr)
        {
            if (sceneSettingsPanel.isCameraEditingSelected)
                sceneSettingsPanel.isCameraEditingSelected = false;

            auto tc = actorContext->GetTransform();
            auto data = tc->GetMatrix().data;

            DrawGizmo(camera, data);

            if (ImGuizmo::IsUsing())
            {
                if (operation == ImGuizmo::TRANSLATE)
                {
                    Math::DecomposePosition(data, tc->GetPosition());
                }
                else if (operation == ImGuizmo::ROTATE)
                {
                    Vector3 delta;
                    Vector3 *old = tc->GetRotation();

                    Math::DecomposeRotation(data, &delta);

                    old->x += delta.x - old->x;
                    old->y += delta.y - old->y;
                    old->z += delta.z - old->z;
                }
                else if (operation == ImGuizmo::SCALE)
                {
                    Math::DecomposeScale(data, tc->GetScale());
                }
            }
        }

        if (sceneSettingsPanel.isCameraEditingSelected && World::GetActive() != nullptr)
        {
            PerspectiveCamera *editCamera = World::GetActive()->GetActorCameraComponent()->camera;
            if (camera != nullptr && editCamera != nullptr)
            {
                sceneHierarchyPanel.selectionContext = nullptr;

                auto data = editCamera->GetTransformMatrix().data;
                DrawGizmo(camera, data);

                if (ImGuizmo::IsUsing())
                {
                    if (operation == ImGuizmo::TRANSLATE)
                    {
                        Math::DecomposePosition(data, editCamera->GetPosition());
                    }
                    else if (operation == ImGuizmo::ROTATE)
                    {
                        Vector3 delta;
                        Vector3 *old = editCamera->GetRotation();

                        Math::DecomposeRotation(data, &delta);

                        old->x += delta.x - old->x;
                        old->y += delta.y - old->y;
                        old->z += delta.z - old->z;
                    }
                }
            }
        }

        ImGui::End();
    }

    void EditorLayer::OpenScene(const std::string &name)
    {
        SceneSerializer ser{World::GetActive()};
        ser.DeserializeAndCreateNewScene(name);
        SetContexts();
        World::InitActive();
    }

    void EditorLayer::SetContexts()
    {
        sceneHierarchyPanel.UpdateContextToWorldActive();
        sceneSettingsPanel.UpdateSceneToWorldActive();
    }

    void EditorLayer::DrawGizmo(PerspectiveCamera *camera, float *data)
    {
        if (camera != nullptr && data != nullptr)
        {
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

            ImGuizmo::Manipulate(camera->GetViewMatrix().data, camera->GetProjection()->data, operation, ImGuizmo::LOCAL, data);
        }
    }
}