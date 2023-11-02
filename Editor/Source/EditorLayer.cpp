#include "EditorLayer.h"

#include "Panels/CoreEditorUtils.h"

#define CE_EDITOR_CAM_NAME "__EditorCamera__"

#define IM_TEXT_UTIL(a, b, s) ImGui::InputText(b, a, s)

namespace Core
{
    //? EDITOR VARIABLES
    static EditorLayer *inst;

    void EditorLayer::OnAttach()
    {
        // Create editor camera
        CameraSystem::Generate(CE_EDITOR_CAM_NAME, Math::DegToRad(90), Engine::GetWindowAspect(), 0.01f, 1000.0f);
        CameraSystem::Activate(CE_EDITOR_CAM_NAME);

        state.imageViewerImage = nullptr;
        state.editMaterialTexture = nullptr;
        state.EditorScene = nullptr;
        state.editMaterialTextureSizes = 50.0f;

        state.movement = new PerspectiveMovement();
        state.IconPlayTexture = new Texture();
        state.IconPlayTexture->Load("EngineResources/CeImage/Icons/PlayButton.ce_image");

        state.IconStopTexture = new Texture();
        state.IconStopTexture->Load("EngineResources/CeImage/Icons/StopButton.ce_image");
        contentBrowserPanel.LoadAssets();

        SetContexts();
        StopSceneRuntime();
        SwapActiveCameraTo(CameraEditor);

        // Runtime loading from project
        if (Project::GetConfig() != nullptr)
            OpenScene(Project::GetConfig()->startScene);
        else
            New();

        state.editProjectState.Clear();

        inst = this;
    }

    void EditorLayer::OnDetach()
    {
        delete state.IconPlayTexture;
        delete state.IconStopTexture;
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
        UI_DrawImageViewer();
        UI_DrawEditProjectConfiguration();
        UI_DrawEditShaderFile();
        UI_DrawEngineStats();
        UI_DrawTestButtons();

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
        {
            state.movement->UpdateRotation(CameraSystem::GetActive());

            // TODO: Check if the mouse is in the viewport
            if (state.updateCameraWithMouse)
            {
                state.movement->UpdatePosition(CameraSystem::GetActive());
                state.movement->UpdateWithMouse(CameraSystem::GetActive());
            }
        }

        // key binds
        bool shift = Input::GetKey(Keys::LeftShift);
        bool ctrl = Input::GetKey(Keys::LeftControl);
        bool alt = Input::GetKey(Keys::LeftAlt);

        switch (dropDownSetting)
        {
        case DropDownSettingProject:
            break;

        case DropDownSettingLibrary:
            break;

        case DropDownSettingNone:
        case DropDownSettingScene:
        default:
            if (ctrl && Input::GetKeyJustNow(Keys::N))
                New();

            if (ctrl && Input::GetKeyJustNow(Keys::O))
                Open();

            if (ctrl && shift && Input::GetKeyJustNow(Keys::S))
                SaveAs();

            if (ctrl && Input::GetKeyJustNow(Keys::S))
                Save();
            break;
        }

        if (Input::GetButton(Buttons::Right))
        {
            if (!state.updateCameraWithMouse)
            {
                Vector2 position = Input::GetMousePosition();
                if (position.x > state.viewportLeftTop.x &&
                    position.y > state.viewportLeftTop.y &&
                    position.x < state.viewportRightBottom.x + state.viewportLeftTop.x &&
                    position.y < state.viewportRightBottom.y + state.viewportLeftTop.y)
                    state.updateCameraWithMouse = true;
            }
        }
        else
        {
            state.updateCameraWithMouse = false;
        }

        // Dupe an actor`
        if (sceneHierarchyPanel.selectionContext != nullptr)
        {
            if (ctrl && Input::GetKeyJustNow(Keys::D))
            {
                World::GetActive()->AddActor(Actor::From(sceneHierarchyPanel.selectionContext));
                sceneHierarchyPanel.selectionContext = nullptr;
            }

            if (Input::GetKeyJustNow(Keys::Delete))
            {
                sceneHierarchyPanel.RmChildInHierarchy(sceneHierarchyPanel.selectionContext->GetParent(), sceneHierarchyPanel.selectionContext->GetUUID());
                sceneHierarchyPanel.selectionContext = nullptr;
            }
        }
    }
    // --------------------------------

    // -- UI ----------------------------
    void EditorLayer::UI_DrawTopPlayStopBar()
    {
        ImGui::Begin("##topbar");

        Texture *tex = currentSceneState == SceneStatePlay ? state.IconStopTexture : state.IconPlayTexture;
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
        if (!state.drawEditMaterial)
            return;

        if (currentSceneState == SceneStatePlay)
            return;

        ImGui::Begin("Edit Material");
        ImGui::Text("Material Name: %s", state.materialConfigurationToEdit.name.c_str());

        // Color
        auto color = &state.materialConfigurationToEdit.color;
        float colors[4] = {color->r / 255, color->g / 255, color->b / 255, color->a / 255};
        if (ImGui::ColorEdit4("Color", colors))
            color->Set4(colors, 255);

        // -- TEXTURE -------------------------
        if (state.materialConfigurationToEdit.colorTextureName.empty())
        {
            ImGui::Button("Texture");
        }
        else
        {
            if (state.editMaterialTexture && state.editMaterialTexture->HasImage())
            {
                Texture *tex = state.editMaterialTexture;
                float aspect = (float)tex->GetWidth() / (float)tex->GetHeight();
                ImGui::Image((void *)(CeU64)(CeU32)tex->GetID(), {aspect * state.editMaterialTextureSizes, aspect * state.editMaterialTextureSizes});
            }
        }

        if (ImGui::BeginDragDropTarget())
        {
            const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CE_CONTENT_PANEL");

            if (payload)
            {
                const char *name = (const char *)payload->Data;
                if (name)
                {
                    if (StringUtils::FileExtIsImage(name))
                    {
                        state.materialConfigurationToEdit.colorTextureName = name;
                        if (!state.materialConfigurationToEdit.colorTextureName.empty())
                        {
                            delete state.editMaterialTexture;
                            state.editMaterialTexture = new Texture();
                            state.editMaterialTexture->Load(name);
                        }
                    }
                }
            }
            ImGui::EndDragDropTarget();
        }

        if (!state.materialConfigurationToEdit.colorTextureName.empty())
        {
            if (ImGui::Button("Remove Texture"))
            {
                state.materialConfigurationToEdit.colorTextureName = "";
                if (state.editMaterialTexture)
                {
                    delete state.editMaterialTexture;
                    state.editMaterialTexture = nullptr;
                }
            }
        }

        ImGui::SameLine();
        // ------------------------------------

        if (ImGui::Button("Close"))
        {
            EditorUtils::MaterialToFile(state.materialConfigurationToEdit.name, &state.materialConfigurationToEdit);

            delete state.editMaterialTexture;
            state.editMaterialTexture = nullptr;

            // Change
            auto out = MaterialManager::Get(state.materialConfigurationToEdit.name);
            out->GetColor()->Set(&state.materialConfigurationToEdit.color);
            out->SetColorTexture(state.materialConfigurationToEdit.colorTextureName);
            MaterialManager::Release(state.materialConfigurationToEdit.name);
            // End Change

            MaterialManager::Release(state.materialConfigurationToEdit.name);
            state.drawEditMaterial = false;
        }

        ImGui::End();
    }

    void EditorLayer::UI_DrawImageViewer()
    {

        if (!state.drawImageViewer)
            return;

        if (state.imageViewerImage == nullptr)
            return;

        ImGui::Begin("Image Viewer");

        if (ImGui::Button("Close"))
        {
            delete state.imageViewerImage;
            state.drawImageViewer = false;

            ImGui::End();
            return;
        }

        ImGui::SameLine();

        ImGui::DragFloat("Scaling", &state.imageViewerScale, 0.05, 0.01);

        ImGui::Image((void *)(CeU64)(CeU32)state.imageViewerImage->GetID(), {(float)state.imageViewerImage->GetWidth() * state.imageViewerScale, (float)state.imageViewerImage->GetHeight() * state.imageViewerScale});

        if (ImGui::BeginDragDropTarget())
        {
            const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CE_CONTENT_PANEL");

            if (payload)
            {
                const char *name = (const char *)payload->Data;
                if (name)
                {
                    if (StringUtils::FileExtIsImage(name))
                    {
                        state.imageViewerImage = new Texture();
                        state.imageViewerImage->Load(name);

                        state.drawImageViewer = true;
                    }
                }
            }

            ImGui::EndDragDropTarget();
        }
        ImGui::End();
    }

    void EditorLayer::UI_DrawEditProjectConfiguration()
    {
        auto config = Project::GetConfig();
        if (!state.editProjectState.Draw || !config) // NOTE: No config is no project.
            return;

        ImGui::Begin("Edit Project Configuration");

        // WIP: Text fields
        IM_TEXT_UTIL(state.editProjectState.Name, "Name", 256);
        IM_TEXT_UTIL(state.editProjectState.AssetPath, "Asset Path", 256);
        IM_TEXT_UTIL(state.editProjectState.StartScene, "Start Scene", 256);
        IM_TEXT_UTIL(state.editProjectState.ScriptPath, "Script Path", 256);
        IM_TEXT_UTIL(state.editProjectState.BuildScriptPath, "Build Script Path", 256);
        IM_TEXT_UTIL(state.editProjectState.ScriptFilesPath, "Script Files Path", 256);

        if (ImGui::Button("Done"))
        {
            //? Set project configs
            config->name = state.editProjectState.Name;
            config->assetPath = state.editProjectState.AssetPath;
            config->startScene = state.editProjectState.StartScene;
            config->scriptPath = state.editProjectState.ScriptPath;
            config->buildScriptsPath = state.editProjectState.BuildScriptPath;
            config->scriptFilesPath = state.editProjectState.ScriptFilesPath;

            ProjectSerializer ser(Project::GetInstance());
            ser.Serialize("Project.ce_proj");

            Project::Load("Project.ce_proj");
            state.editProjectState.Draw = false;
            state.editProjectState.Clear();
        }

        ImGui::End();
    }

    void EditorLayer::UI_DrawEditShaderFile()
    {
        // if (!state.editShaderFile.Draw)
        //     return;

        // ImGui::Begin("Shader File Edit");

        // ImGui::InputTextMultiline("Shader", state.editShaderFile.FileContent, sizeof(state.editShaderFile.FileContent) * sizeof(char), {1000, 500});

        // if (ImGui::Button("Close"))
        // {
        //     state.editShaderFile.Draw = false;
        // }

        // ImGui::End();
    }

    void EditorLayer::UI_DrawEngineStats()
    {
        float delta = Engine::GetDeltaTime();
        float fps = Engine::GetFPS();

        ImGui::Begin("Engine Stats");
        ImGui::Text("%.3f : DeltaTime", delta);
        ImGui::Text("%i : FPS", (int)fps);

        auto pos = Input::GetMousePosition();
        ImGui::Text("%i / %i : Mouse Position", (int)pos.x, (int)pos.y);
        ImGui::Text("%i / %i by %i / %i : Bounds", (int)state.viewportLeftTop.x, (int)state.viewportLeftTop.y, (int)state.viewportRightBottom.x, (int)state.viewportRightBottom.y);

        // Popup for viewing things
        const int selCount = 5;
        const char *selections[] = {"Full", "Color", "Texture", "Normal", "Light"};
        const char *selection = selections[(int)Renderer::GetRenderMode()];

        if (ImGui::BeginCombo("Render Mode", selection))
        {
            for (int i = 0; i < selCount; i++)
            {
                bool isSelected = (selection == selections[i]);

                if (ImGui::Selectable(selections[i], isSelected))
                {
                    selection = selections[i];
                    Renderer::SetRenderMode((RenderMode)i);
                }

                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        ImGui::End();
    }

    void EditorLayer::UI_DrawTestButtons()
    {
        ImGui::Begin("Tests");

        int shaderAddCount = 999;
        if (ImGui::Button("Start The Shader Test"))
        {
            for (int i = 0; i < shaderAddCount; i++)
            {
                World::GetActive()->GetEnvironment()->sky->AddShaderData(sizeof(Color), new Color(255, 255, 255, 255), SkyShaderDataType::Color, std::string("Test" + i));
            }
        }

        if (ImGui::Button("Stop The Shader Test"))
        {
            for (int i = 0; i < shaderAddCount; i++)
            {
                World::GetActive()->GetEnvironment()->sky->RemoveSkyShaderDataByName(std::string("Test" + i));
            }
        }

        ImGui::End();
    }

    void EditorLayer::UI_DrawMainTopBar()
    {

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::MenuItem("Scene"))

                ImGui::OpenPopup("ScenePopup");

            if (ImGui::MenuItem("Project"))
                ImGui::OpenPopup("ProjectPopup");

            if (ImGui::MenuItem("Library"))
                ImGui::OpenPopup("LibraryPopup");

            if (ImGui::BeginPopup("ScenePopup"))
            {
                if (ImGui::MenuItem("New", "Ctrl+N"))
                    New();

                if (ImGui::MenuItem("Open...", "Ctrl+O"))
                    Open();

                if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
                    SaveAs();

                ImGui::EndPopup();

                dropDownSetting = DropDownSettingScene;
            }

            if (ImGui::BeginPopup("ProjectPopup"))
            {
                if (ImGui::MenuItem("New", "Ctrl+N"))
                    NewProject();

                if (ImGui::MenuItem("Open...", "Ctrl+O"))
                    OpenProject();

                if (ImGui::MenuItem("Save", "Ctrl+S"))
                    SaveProject();

                if (ImGui::MenuItem("Edit Configuration"))
                {
                    state.editProjectState.Draw = true;
                    CeMemory::Copy(&state.editProjectState.Name, Project::GetConfig()->name.c_str(), 256);
                    CeMemory::Copy(&state.editProjectState.AssetPath, Project::GetConfig()->assetPath.c_str(), 256);
                    CeMemory::Copy(&state.editProjectState.ScriptPath, Project::GetConfig()->scriptPath.c_str(), 256);
                    CeMemory::Copy(&state.editProjectState.StartScene, Project::GetConfig()->startScene.c_str(), 256);
                    CeMemory::Copy(&state.editProjectState.BuildScriptPath, Project::GetConfig()->buildScriptsPath.c_str(), 256);
                    CeMemory::Copy(&state.editProjectState.ScriptFilesPath, Project::GetConfig()->scriptFilesPath.c_str(), 256);
                }

                ImGui::EndPopup();

                dropDownSetting = DropDownSettingProject;
            }

            if (ImGui::BeginPopup("LibraryPopup"))
            {
                if (ImGui::MenuItem("Re-Build"))
                {
                    ReBuildLibrary();
                }

                dropDownSetting = DropDownSettingLibrary;

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
        World::Create("Untitled Scene");
        World::Activate("Untitled Scene");
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
        state.activeScenePath = name;
        Save();
    }

    void EditorLayer::Save()
    {
        if (!state.activeScenePath.empty())
        {
            SceneSerializer ser{World::GetActive()};
            ser.Serialize(state.activeScenePath);
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

    void EditorLayer::ReBuildLibrary()
    {
        auto config = Project::GetConfig();

        if (!config)
            return;

        ScriptEngine::UnloadLibrary();

        std::string finalScript;

        finalScript += "call " + config->buildScriptsPath;

        int sys = system(finalScript.c_str());
        if (sys == 0)
            ScriptEngine::LoadGameLibrary(config->scriptPath);
    }
    // --------------------------------

    // -- EDITOR GENERAL ACTIONS ------

    void EditorLayer::MapGizmoToKey(Keys key, int newMode)
    {
        if (Input::GetKey(key))
            state.operation = (ImGuizmo::OPERATION)newMode;
    }

    void EditorLayer::StartSceneRuntime()
    {
        state.lastPLightCount = PointLight::GetPointLightGID();
        PointLight::SetGlobalID0();

        currentSceneState = SceneStatePlay;
        state.EditorScene = World::CopyActive();
        SwapActiveCameraTo(CameraGamePlay);
        World::StartActive();
    }

    void EditorLayer::StopSceneRuntime()
    {

        currentSceneState = SceneStateStop;
        World::StopActive();

        if (state.EditorScene != nullptr)
        {
            World::CopyToActive(state.EditorScene);

            state.EditorScene->Destroy();

            delete state.EditorScene;
            PointLight::SetGlobalID0();
            for (int i = 0; i < state.lastPLightCount; i++)
                PointLight::IncremenetGolbalID();

            SetContexts();
        }

        sceneHierarchyPanel.selectionContext = nullptr;

        SwapActiveCameraTo(CameraEditor);

        World::InitActive();
    }

    void EditorLayer::SwapActiveCameraTo(ActiveCameraType type)
    {
        if (activeCameraType != type)
        {
            if (type == CameraEditor)
                CameraSystem::Activate(CE_EDITOR_CAM_NAME);
            else
                World::GetActive()->ActivateSceneCamera();

            activeCameraType = type;

            ResizeViewport();
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
        Renderer::Resize(state.lastFrameViewportSize.x, state.lastFrameViewportSize.y);
    }

    EditorLayer *EditorLayer::Get()
    {
        return inst;
    }

    void EditorLayer::OpenScene(const std::string &name)
    {
        state.activeScenePath = name;
        SceneSerializer ser{World::GetActive()};
        ser.DeserializeAndCreateNewScene(name);
        SetContexts();
        World::InitActive();
    }
    // --------------------------------

    // -- VIEWPORT---------------------
    void EditorLayer::BeginDockspace()
    {
        if (state.opt_fullscreen)
        {
            ImGuiViewport *viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            state.window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            state.window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        if (state.dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            state.window_flags |= ImGuiWindowFlags_NoBackground;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &state.dockspaceOpen, state.window_flags);
        ImGui::PopStyleVar();

        if (state.opt_fullscreen)
            ImGui::PopStyleVar(2);

        // DockSpace
        ImGuiIO &io = ImGui::GetIO();
        ImGuiStyle &style = ImGui::GetStyle();
        float minWinSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x = 250.0f;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), state.dockspace_flags);
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
        if (viewportSize.x != state.lastFrameViewportSize.x || viewportSize.y != state.lastFrameViewportSize.y)
        {
            state.lastFrameViewportSize = viewportSize;
            ResizeViewport();
        }

        state.viewportLeftTop = ImGui::GetWindowPos();
        state.viewportRightBottom = ImGui::GetWindowSize();

        // End update renderer viewport
        ImGui::Image((void *)(CeU64)(CeU32)(Renderer::GetFrameBuffer()->GetRenderPass(0)->id), viewportSize, ImVec2{0, 1}, ImVec2{1, 0});

        if (ImGui::BeginDragDropTarget())
        {
            const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CE_CONTENT_PANEL");

            if (payload)
            {
                const char *name = (const char *)payload->Data;
                auto ext = StringUtils::GetFileExtension(name);
                if (name)
                {
                    if (ext == "ce_scene")
                        OpenScene(name);
                    else if (ext == "ce_mat")
                    {
                        auto mat = MaterialManager::Get(name);
                        state.materialConfigurationToEdit.name = name;
                        state.materialConfigurationToEdit.color.Set(mat->GetColor());
                        state.materialConfigurationToEdit.colorTextureName = mat->GetColorTexturePath();
                        if (!state.materialConfigurationToEdit.colorTextureName.empty())
                        {
                            state.editMaterialTexture = new Texture();
                            state.editMaterialTexture->Load(state.materialConfigurationToEdit.colorTextureName);
                        }
                        else
                        {
                            state.editMaterialTexture = nullptr;
                        }

                        state.drawEditMaterial = true;
                    }
                    else if (StringUtils::FileExtIsImage(name))
                    {
                        state.imageViewerImage = new Texture();
                        state.imageViewerImage->Load(name);

                        state.drawImageViewer = true;
                    }
                    else if (ext == "glsl")
                    {
                        // TODO: Fix-me
                        //  state.editShaderFile.FileName = (char *)name;
                        //  state.editShaderFile.FileContent = (char *)FileSystem::ReadFileContent(name).c_str();
                        //  state.editShaderFile.Draw = true;
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
                auto tc = actorContext->GetTransform();
                auto delta = actorContext->GetParent() ? Matrix4::Invert(actorContext->GetParent()->GetWorldMatrix()).data : NULL;
                auto data = actorContext->GetWorldMatrix()->data;
                DrawGizmo(camera, data, delta);

                if (ImGuizmo::IsUsing())
                {
                    Matrix4 dater;
                    if (actorContext->GetParent() != nullptr)
                    {
                        Matrix4 dataMatrix = Matrix4();
                        dataMatrix.From(actorContext->GetLocalMatrix());

                        Matrix4 deltaMatrix = Matrix4();
                        if (delta)
                            deltaMatrix.From(delta);
                        else
                            deltaMatrix = Matrix4::Identity();

                        dater = Matrix4::Multiply(dataMatrix, deltaMatrix);
                    }
                    else
                    {
                        dater.From(data);
                    }

                    float matrixTranslation[3], matrixRotation[3], matrixScale[3];
                    ImGuizmo::DecomposeMatrixToComponents(dater.data, matrixTranslation, matrixRotation, matrixScale);
                    ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, dater.data);

                    if (state.operation == ImGuizmo::TRANSLATE)
                    {
                        matrixTranslation[0] = dater.data[12];
                        matrixTranslation[1] = dater.data[13];
                        matrixTranslation[2] = dater.data[14];
                        tc->position.Set(matrixTranslation[0], matrixTranslation[1], matrixTranslation[2]);
                    }
                    else if (state.operation == ImGuizmo::ROTATE)
                    {
                        tc->rotation.Set(Math::DegToRad(matrixRotation[0]), Math::DegToRad(matrixRotation[1]), Math::DegToRad(matrixRotation[2]));
                    }
                    else if (state.operation == ImGuizmo::SCALE)
                    {
                        tc->scale.Set(matrixScale[0], matrixScale[1], matrixScale[2]);
                    }
                }
            }
        }

        ImGui::End();
    }

    void EditorLayer::DrawGizmo(PerspectiveCamera *camera, float *data, float *deltaMatrix)
    {
        if (camera != nullptr && data != nullptr)
        {
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

            ImGuizmo::Manipulate(camera->GetViewMatrix().data, camera->GetProjection()->data, state.operation, ImGuizmo::WORLD, data, deltaMatrix);
        }
    }

    void EditProjectState::Clear()
    {
        CeMemory::Zero(&this->Name, 256);
        CeMemory::Zero(&this->AssetPath, 256);
        CeMemory::Zero(&this->BuildScriptPath, 256);
        CeMemory::Zero(&this->ScriptFilesPath, 256);
        CeMemory::Zero(&this->ScriptPath, 256);
        CeMemory::Zero(&this->StartScene, 256);
    }

    // --------------------------------
}