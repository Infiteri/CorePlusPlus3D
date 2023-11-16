#pragma once

#include "Core.h"

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/SceneSettingsPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/LoggerPanel.h"

#include <vector>
#include <ImGuizmo.h>

namespace Core
{
    struct EditProjectState
    {
        char Name[256];
        char StartScene[256];
        char AssetPath[256];
        char ScriptPath[256];
        char ScriptFilesPath[256];
        char BuildScriptPath[256];
        bool Draw = false;

        void Clear();
    };

    struct UIAssetViewer
    {
        bool render = true;
    };

    struct EditShaderFile
    {
        char *FileName;
        char *FileContent;
        bool Draw = false;
    };

    struct EditorState
    {
        EditProjectState editProjectState;
        EditShaderFile editShaderFile;

        bool drawEditMaterial = false;
        bool drawImageViewer = false;

        Texture *imageViewerImage;
        Texture *editMaterialTexture;
        float editMaterialTextureSizes;
        float imageViewerScale = 1.0f;

        std::string activeScenePath;

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
        CeU32 lastPLightCount;
        CeU32 spotLightCount;

        ImVec2 viewportLeftTop;
        ImVec2 viewportRightBottom;

        bool updateCameraWithMouse = false;

        UIAssetViewer assetViewer;
    };

    class EditorLayer : public Layer
    {
    public:
        EditorState state;
        SceneHierarchyPanel sceneHierarchyPanel;
        SceneSettingsPanel sceneSettingsPanel;
        ContentBrowserPanel contentBrowserPanel;
        LoggerPanel loggerPanel;

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
        void Save();

        void NewProject();
        void OpenProject();
        void SaveProject();

        void ReBuildLibrary();
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

        enum ActiveDropDownMenuSetting
        {
            DropDownSettingNone,
            DropDownSettingScene,
            DropDownSettingProject,
            DropDownSettingLibrary,
        };

        SceneState currentSceneState = SceneStateStop;
        ActiveCameraType activeCameraType = CameraEditor;
        ActiveDropDownMenuSetting dropDownSetting = DropDownSettingNone;

        void StartSceneRuntime();
        void StopSceneRuntime();
        void SwapActiveCameraTo(ActiveCameraType type);
        void HandleViewGameCamera(bool flag);

        // -------------- UI ------------------
        void UI_DrawMainTopBar();
        void UI_DrawTopPlayStopBar();
        void UI_DrawEditMaterial();
        void UI_DrawImageViewer();
        void UI_DrawEditProjectConfiguration();
        void UI_DrawEditShaderFile();
        void UI_DrawEngineStats();
        void UI_DrawTestButtons();
        void UI_DrawAssetLayout();
        void UI_DrawAssetViewer();

        void UI_UTIL_DrawFolderContents(const std::string& folderPath, const std::string& parentPath);
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
        void DrawGizmo(PerspectiveCamera *camera, float *data, float *deltaMatrix = NULL);
    };
}