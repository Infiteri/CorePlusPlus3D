#pragma once

#include "Core/Layer/Layer.h"

#include "Panels/SceneHierarchyPanel.h"

namespace Core
{
    class EditorLayer : public Layer
    {
    public:
        SceneHierarchyPanel sceneHierarchyPanel;

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