#pragma once

#include "Scene/Scene.h"
#include "Scene/World.h"

namespace Core
{
    class SceneSettingsPanel
    {
    private:
        Scene *scene = nullptr;

    public:
        SceneSettingsPanel();
        ~SceneSettingsPanel();

        inline void UpdateSceneToWorldActive() { scene = World::GetActive(); };

        void OnImGuiRender();
    };
}