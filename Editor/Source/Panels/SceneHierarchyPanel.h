#pragma once

#include "Scene/Scene.h"
#include "Panel.h"


namespace Core
{
    class Actor;

    class SceneHierarchyPanel : public Panel
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