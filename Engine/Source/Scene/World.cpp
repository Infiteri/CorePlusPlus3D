#include "World.h"

#include "Core/Logger.h"

#include <unordered_map>

namespace Core
{
    static std::unordered_map<std::string, Scene *> scenes;
    static Scene *activeScene;
    ;

    void World::Init()
    {
    }

    void World::Shutdown()
    {
        for (auto it = scenes.begin(); it != scenes.end(); it++)
        {
            auto scene = it->second;
            if (scene->GetName() == activeScene->GetName())
                continue;
            scene->Destroy();
            delete scene;
        }

        if (activeScene)
        {
            activeScene->Destroy();
            delete activeScene;
            activeScene = nullptr;
        }

        scenes.clear();
    }

    Scene *World::Get(const std::string &name)
    {
        return scenes[name];
    }

    Scene *World::GetActive()
    {
        return activeScene;
    }

    Scene *World::Create(const std::string &name)
    {
        Scene *scene = new Scene();
        scene->SetName(name.c_str());
        scenes[name] = scene;

        return scene;
    }

    void World::Activate(const std::string &name)
    {
        if (!scenes[name])
        {
            CE_ERROR("Unable to find scene '%s'.", name.c_str());
            return;
        }

        activeScene = scenes[name];
    }

    void World::Delete(const std::string &name)
    {
        Scene *scene = scenes[name];
        if (scene)
        {
            scene->Destroy();
            delete scene;
            scene = nullptr;
        }

        if (activeScene->GetName().compare(name) == 0)
        {
            activeScene->Destroy();
            delete activeScene;
            activeScene = nullptr;
        }
    }

    void World::InitActive()
    {
        if (activeScene)
            activeScene->Init();
    }

    void World::StartActive()
    {
        if (activeScene)
            activeScene->Start();
    }

    void World::UpdateActive()
    {
        if (activeScene)
            activeScene->Update();
    }

    void World::RenderActive()
    {
        if (activeScene)
            activeScene->Render();
    }

    void World::StopActive()
    {
        if (activeScene)
            activeScene->Stop();
    }

    void World::DestroyActive()
    {
        if (activeScene)
        {
            activeScene->Destroy();
            delete activeScene;
        }

        activeScene = nullptr;
    }
}