#include "EditorLayer.h"

#include "Panels/CoreEditorUtils.h"

#include <ImGuizmo.h>

namespace Core
{
    //? EDITOR VARIABLES
    static EditorLayer *inst;
    static bool drawEditMaterial = false;
    static MaterialConfiguration materialConfigurationToEdit;
    static Scene *EditorScene;
    static Texture *IconPlayTexture;
    static Texture *IconStopTexture;
    static ImGuizmo::OPERATION operation = ImGuizmo::TRANSLATE;
    static PerspectiveMovement *movement;

    //? DOCKSPACE VARIABLES
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

        IconPlayTexture = new Texture();
        IconPlayTexture->Load("EngineResources/CeImage/Icons/PlayButton.ce_image");

        IconStopTexture = new Texture();
        IconStopTexture->Load("EngineResources/CeImage/Icons/StopButton.ce_image");

        SetContexts();
        contentBrowserPanel.LoadAssets();

        movement = new PerspectiveMovement();

        currentSceneState = SceneStateStop;
        StopSceneRuntime();

        SwapActiveCameraTo(CameraEditor);

        // Runtime loading from project
        if (Project::GetConfig() != nullptr)
        {
            OpenScene(Project::GetConfig()->startScene);
        }
        else
        {
            New();
            World::InitActive();
        }

        inst = this;
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
            ResizeViewport();
    }

    void EditorLayer::OnImGuiRender()
    {
        BeginDockspace();

        sceneHierarchyPanel.OnImGuiRender();
        sceneSettingsPanel.OnImGuiRender();
        contentBrowserPanel.OnImGuiRender();

        UI_DrawEditMaterial();
        UI_DrawMainTopBar();
        UI_DrawTopPlayStopBar();
        RenderSceneViewport();

        EndDockspace();

        MapGizmoToKey(Keys::T, ImGuizmo::TRANSLATE);
        MapGizmoToKey(Keys::R, ImGuizmo::ROTATE);
        MapGizmoToKey(Keys::E, ImGuizmo::SCALE);
    }

    // -- EDITOR UPDATING METHODS -----
    void EditorLayer::OnUpdateRuntime()
    {
    }

    void EditorLayer::OnUpdateEditor()
    {
        if (activeCameraType == CameraEditor)
            movement->Update(CameraSystem::GetActive());
    }
    // --------------------------------

    // -- UI ----------------------------
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

    void EditorLayer::UI_DrawEditMaterial()
    {
        if (!drawEditMaterial)
            return;

        if (currentSceneState == SceneStatePlay)
            return;

        ImGui::Begin("Edit Material");
        ImGui::Text("Material Name: %s", materialConfigurationToEdit.name.c_str());

        // Color
        auto color = &materialConfigurationToEdit.color;
        float colors[4] = {color->r / 255, color->g / 255, color->b / 255, color->a / 255};
        if (ImGui::ColorEdit4("Color", colors))
            color->Set4(colors, 255);

        if (ImGui::Button("Texture"))
        {
        }

        if (ImGui::BeginDragDropTarget())
        {
            const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CE_CONTENT_PANEL");

            if (payload)
            {
                const char *name = (const char *)payload->Data;
                if (name)
                {
                    std::string ext = StringUtils::GetFileExtension(name);
                    if (ext == "png" || ext == "jpg" || ext == "ce_image")
                        materialConfigurationToEdit.colorTextureName = name;
                }
            }

            ImGui::EndDragDropTarget();
        }

        if (ImGui::Button("Close"))
        {
            EditorUtils::MaterialToFile(materialConfigurationToEdit.name, &materialConfigurationToEdit);

            // Change
            auto out = MaterialManager::Get(materialConfigurationToEdit.name);
            out->GetColor()->Set(&materialConfigurationToEdit.color);
            out->SetColorTexture(materialConfigurationToEdit.colorTextureName);
            MaterialManager::Release(materialConfigurationToEdit.name);
            // End Change

            MaterialManager::Release(materialConfigurationToEdit.name);
            drawEditMaterial = false;
        }

        ImGui::End();
    }

    void EditorLayer::UI_DrawMainTopBar()
    {

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::MenuItem("File"))
                ImGui::OpenPopup("FilePopup");

            if (ImGui::MenuItem("Project"))
                ImGui::OpenPopup("ProjectPopup");

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

            if (ImGui::BeginPopup("ProjectPopup"))
            {
                if (ImGui::MenuItem("New", "Ctrl+N"))
                    NewProject();

                if (ImGui::MenuItem("Open...", "Ctrl+O"))
                    OpenProject();

                if (ImGui::MenuItem("Save", "Ctrl+S"))
                    SaveProject();

                ImGui::EndPopup();
            }

            ImGui::EndMainMenuBar();
        }
    }
    // --------------------------------

    // -- TOP BAR ACTIONS -------------
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

    void EditorLayer::NewProject()
    {
        Project::New();
    }

    void EditorLayer::OpenProject()
    {
        std::string name = Platform::OpenFileDialog("Core Project(*.ce_proj)\0*.ce_proj\0");
        if (!name.empty())
            Project::Load(name);
    }

    void EditorLayer::SaveProject()
    {
        Project::SaveActive("Project.ce_proj");
    }
    // --------------------------------

    // -- EDITOR GENERAL ACTIONS ------

    void EditorLayer::MapGizmoToKey(Keys key, int newMode)
    {
        if (Input::GetKey(key))
            operation = (ImGuizmo::OPERATION)newMode;
    }

    void EditorLayer::StartSceneRuntime()
    {
        currentSceneState = SceneStatePlay;
        EditorScene = World::GetActive() ? Scene::GetCopyOfScene(World::GetActive()) : nullptr;
        SwapActiveCameraTo(CameraGamePlay);
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

        SwapActiveCameraTo(CameraEditor);

        World::InitActive();
    }

    void EditorLayer::SwapActiveCameraTo(ActiveCameraType type)
    {
        if (activeCameraType != type)
        {
            if (type == CameraEditor)
                CameraSystem::Activate("__EditorCamera__");
            else
                World::GetActive()->ActivateSceneCamera();

            activeCameraType = type;
        }
    }

    void EditorLayer::HandleViewGameCamera(bool flag)
    {
        if (flag)
            SwapActiveCameraTo(CameraGamePlay);
        else
            SwapActiveCameraTo(CameraEditor);
    }

    void EditorLayer::SetContexts()
    {
        sceneHierarchyPanel.UpdateContextToWorldActive();
        sceneSettingsPanel.UpdateSceneToWorldActive();
    }

    void EditorLayer::ResizeViewport()
    {
        Renderer::Resize(lastFrameViewportSize.x, lastFrameViewportSize.y);
    }

    EditorLayer *EditorLayer::Get()
    {
        return inst;
    }

    void EditorLayer::OpenScene(const std::string &name)
    {
        SceneSerializer ser{World::GetActive()};
        ser.DeserializeAndCreateNewScene(name);
        SetContexts();
        World::InitActive();
    }
    // --------------------------------

    // -- VIEWPORT---------------------
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
            ResizeViewport();
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
                {
                    if (StringUtils::GetFileExtension(name).compare("ce_scene") == 0)
                        OpenScene(name);
                    else if (StringUtils::GetFileExtension(name).compare("ce_mat") == 0)
                    {
                        auto mat = MaterialManager::Get(name);
                        materialConfigurationToEdit.name = name;
                        materialConfigurationToEdit.color.Set(mat->GetColor());
                        materialConfigurationToEdit.colorTextureName = mat->GetColorTexture()->HasImage() ? mat->GetColorTexture()->GetImagePath() : "";
                        drawEditMaterial = true;
                    }
                }
            }

            ImGui::EndDragDropTarget();
        }

        if (currentSceneState != SceneStatePlay)
        {
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
        }

        ImGui::End();
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
    // --------------------------------
}