#pragma once

#include "Scene/Scene.h"
#include "Scene/World.h"
#include "Panel.h"


namespace Core
{
    class SceneSettingsPanel : public Panel
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