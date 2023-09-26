#pragma once

#include "Scene/Scene.h"

#include <imgui.h>

namespace Core
{
    class Actor;

    class SceneHierarchyPanel
    {
    private:
        Scene *scene;

    public:
        Actor *selectionContext;

        SceneHierarchyPanel();
        ~SceneHierarchyPanel();

        void UpdateContextToWorldActive();
        void OnImGuiRender();
        void RenderActor(Actor *a);
        void DrawActorComponents(Actor *a);
    };
}