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

        void BeginDockspace();
        void EndDockspace();
        void RenderSceneViewport();
    };
}