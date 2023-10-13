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
        activeScene = nullptr;
    }

    void World::Shutdown()
    {
        for (auto it = scenes.begin(); it != scenes.end(); it++)
        {
            auto scene = it->second;

            if (!scene || scene == nullptr)
                continue;

            if (scene->GetName() == activeScene->GetName())
                continue;

            CE_TRACE("Destroying scene '%s'.", scene->GetName().c_str());

            scene->Destroy();
            delete scene;
        }

        if (activeScene)
        {
            CE_TRACE("Destroying scene '%s'.", activeScene->GetName().c_str());

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
        CE_ASSERT_IF(name.empty() && "Name cannot be empty.");

        if (scenes[name] != nullptr)
        {
            CE_ERROR("Scene '%s' already exists.", name.c_str());
            return nullptr;
        }

        Scene *scene = new Scene();
        scene->SetName(name.c_str());
        scenes[name] = scene;

        CE_DEBUG("Created scene '%s'", name.c_str());

        return scene;
    }

    void World::Activate(const std::string &name)
    {
        CE_ASSERT_IF(name.empty() && "Name cannot be empty.");

        if (!scenes[name])
        {
            CE_ERROR("Unable to find scene '%s'.", name.c_str());
            return;
        }

        activeScene = scenes[name];

        CE_DEBUG("Activated scene '%s'", activeScene->GetName().c_str());
    }

    void World::Delete(const std::string &name)
    {
        CE_ASSERT_IF(name.empty() && "Name cannot be empty.");

        if (activeScene->GetName().compare(name) == 0)
        {
            activeScene = nullptr;
        }

        Scene *scene = scenes[name];
        if (scene)
        {
            auto it = scenes.find(scene->GetName());
            scenes.erase(it);
            CE_TRACE("Deleting scene '%s', scene count is now %i.", name.c_str(), scenes.size());
            scene->Destroy();
            delete scene;
            scene = nullptr;
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