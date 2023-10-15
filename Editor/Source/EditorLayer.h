#pragma once

#include "Core/Layer/Layer.h"

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/SceneSettingsPanel.h"
#include "Panels/ContentBrowserPanel.h"

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
        void OnRender();
        void OnUpdate();
        void OnImGuiRender();
        void OnDetach();
        void OnEvent(Event* event);

        void New();
        void Open();
        void SaveAs();

        enum SceneState
        {
            SceneStatePlay,
            SceneStateStop
        };

        SceneState currentSceneState;

        void StartSceneRuntime();
        void StopSceneRuntime();

        // -------------- UI ------------------
        void UI_DrawTopNavBar();
        void UI_DrawTopBar();
        // ------------------------------------

        void BeginDockspace();
        void EndDockspace();
        void RenderSceneViewport();

        void OpenScene(const std::string& name);
    };
}