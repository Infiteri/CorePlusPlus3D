#pragma once

#include "Core/Layer/Layer.h"

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
        void OnEvent(Event *event);

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
        void UI_DrawMainTopBar();
        void UI_DrawTopPlayStopBar();
        // ------------------------------------

        void BeginDockspace();
        void EndDockspace();
        void RenderSceneViewport();

        void OpenScene(const std::string &name);

    private:
        void SetContexts();
    };
}