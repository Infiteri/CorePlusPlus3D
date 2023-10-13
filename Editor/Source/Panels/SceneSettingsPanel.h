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

        void UpdateSceneToWorldActive();

        void OnImGuiRender();
    };
}