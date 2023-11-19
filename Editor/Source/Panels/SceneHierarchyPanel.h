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
        int actorIndex = 0;

    public:
        Actor *selectionContext;

        SceneHierarchyPanel();
        ~SceneHierarchyPanel();

        void UpdateContextToWorldActive();
        void OnImGuiRender();
        void RenderActor(Actor *a, Actor *parent, bool parentNodeOpen, CeU32 deep);
        void DrawActorComponents(Actor *a);

        Actor *GetChildInHierarchy(Actor *parent, UUID *uid);
        void RmChildInHierarchy(Actor *parent, UUID *uid);
    };
}