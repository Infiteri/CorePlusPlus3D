#pragma once

#include "Core.h"

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/SceneSettingsPanel.h"
#include "Panels/ContentBrowserPanel.h"

#include <vector>
#include <ImGuizmo.h>

namespace Core
{
    struct EditorState
    {
        bool drawEditMaterial = false;
        bool drawImageViewer = false;
        Texture *imageViewerImage;
        Texture *editMaterialTexture;
        float editMaterialTextureSizes;
        float imageViewerScale = 1.0f;

        MaterialConfiguration materialConfigurationToEdit;
        Scene *EditorScene;
        Texture *IconPlayTexture;
        Texture *IconStopTexture;
        ImGuizmo::OPERATION operation = ImGuizmo::TRANSLATE;
        PerspectiveMovement *movement;
        bool dockspaceOpen = true;
        bool opt_fullscreen = true;
        ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        ImVec2 lastFrameViewportSize;
    };

    class EditorLayer : public Layer
    {
    public:
        EditorState state;
        SceneHierarchyPanel sceneHierarchyPanel;
        SceneSettingsPanel sceneSettingsPanel;
        ContentBrowserPanel contentBrowserPanel;

        EditorLayer(){};
        ~EditorLayer(){};

        void OnAttach();
        void OnImGuiRender();
        void OnDetach();
        void OnUpdate();
        void OnEvent(Event *event);

        // -------- UPDATE ROUTINES -----------
        void OnUpdateRuntime();
        void OnUpdateEditor();
        // ------------------------------------

        // -------- TOP BAR ACTIONS -----------
        void New();
        void Open();
        void SaveAs();

        void NewProject();
        void OpenProject();
        void SaveProject();
        // ------------------------------------

        enum SceneState
        {
            SceneStatePlay,
            SceneStateStop
        };

        enum ActiveCameraType
        {
            CameraEditor,
            CameraGamePlay
        };

        SceneState currentSceneState;
        ActiveCameraType activeCameraType = CameraEditor;

        void StartSceneRuntime();
        void StopSceneRuntime();
        void SwapActiveCameraTo(ActiveCameraType type);
        void HandleViewGameCamera(bool flag);

        // -------------- UI ------------------
        void UI_DrawMainTopBar();
        void UI_DrawTopPlayStopBar();
        void UI_DrawEditMaterial();
        void UI_DrawImageViewer();
        // ------------------------------------

        void BeginDockspace();
        void EndDockspace();
        void RenderSceneViewport();

        void OpenScene(const std::string &name);

        static EditorLayer *Get();

    private:
        void MapGizmoToKey(Keys key, int newMode);
        void SetContexts();

        /// @brief VALUES ARE PRE-SET
        void ResizeViewport();
        void DrawGizmo(PerspectiveCamera *camera, float *data);
    };
}