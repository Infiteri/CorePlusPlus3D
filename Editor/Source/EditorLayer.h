#pragma once

#include "Core/Layer/Layer.h"
#include "Renderer/Camera/PerspectiveCamera.h"

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/SceneSettingsPanel.h"
#include "Panels/ContentBrowserPanel.h"

#include <vector>

namespace Core
{
    class EditorLayer : public Layer
    {
    public:
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

        // ------------------------------------
        enum SceneState
        {
            SceneStatePlay,
            SceneStateStop
        };

        SceneState currentSceneState;

        void StartSceneRuntime();
        void StopSceneRuntime();

        // -------------- UI ------------------
        void UI_DrawMainTopBar();
        void UI_DrawTopPlayStopBar();
        // ------------------------------------

        void BeginDockspace();
        void EndDockspace();
        void RenderSceneViewport();

        void OpenScene(const std::string &name);

    private:
        void SetContexts();
        void DrawGizmo(PerspectiveCamera* camera, float* data);
    };
}