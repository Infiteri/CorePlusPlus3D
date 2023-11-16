#include "World.h"

#include "Core/Logger.h"

#include <unordered_map>

namespace Core
{
    static std::unordered_map<std::string, Scene *> scenes;
    static Scene *activeScene = nullptr;
    ;

    void World::Init()
    {
        CE_PROFILE_FUNCTION();

        activeScene = nullptr;
    }

    void World::Shutdown()
    {
        CE_PROFILE_FUNCTION();

        for (auto it = scenes.begin(); it != scenes.end(); it++)
        {
            auto scene = it->second;

            if (!scene || scene == nullptr)
                continue;

            if (scene->GetName() == activeScene->GetName())
                continue;

            CE_CORE_TRACE("Destroying scene '%s'.", scene->GetName().c_str());

            scene->Destroy();
            delete scene;
        }

        if (activeScene)
        {
            CE_CORE_TRACE("Destroying scene '%s'.", activeScene->GetName().c_str());

            activeScene->Destroy();
            delete activeScene;
            activeScene = nullptr;
        }

        scenes.clear();
    }

    Scene *World::Get(const std::string &name)
    {
        CE_PROFILE_FUNCTION();

        return scenes[name];
    }

    Scene *World::GetActive()
    {
        CE_PROFILE_FUNCTION();

        return activeScene;
    }

    Scene *World::Create(const std::string &name)
    {
        CE_PROFILE_FUNCTION();

        CE_ASSERT_IF(name.empty() && "Name cannot be empty.");

        if (scenes[name] != nullptr)
        {
            CE_CORE_ERROR("Scene '%s' already exists.", name.c_str());
            return nullptr;
        }

        Scene *scene = new Scene();
        scene->SetName(name.c_str());
        scenes[name] = scene;

        CE_CORE_DEBUG("Created scene '%s'", name.c_str());

        return scene;
    }

    void World::Activate(const std::string &name)
    {
        CE_PROFILE_FUNCTION();

        CE_ASSERT_IF(name.empty() && "Name cannot be empty.");

        if (!scenes[name])
        {
            CE_CORE_ERROR("Unable to find scene '%s'.", name.c_str());
            return;
        }

        activeScene = scenes[name];

        CE_CORE_DEBUG("Activated scene '%s'", activeScene->GetName().c_str());
    }

    void World::Activate(Scene *scene)
    {
        activeScene = scene;
    }

    Scene *World::CopyActive()
    {
        CE_PROFILE_FUNCTION();

        if (!activeScene)
            return nullptr;

        return Scene::GetCopyOfScene(activeScene);
    }

    void World::CopyToActive(Scene *scene)
    {
        CE_PROFILE_FUNCTION();

        if (!scene)
        {
            CE_CORE_ERROR("World::CopyToActive: Scene cannot be a nullptr.");
            return;
        }

        if (activeScene)
        {
            activeScene->Destroy();
            delete activeScene;
        }

        activeScene = Scene::GetCopyOfScene(scene);
    }

    void World::Delete(const std::string &name)
    {
        CE_PROFILE_FUNCTION();

        CE_ASSERT_IF(name.empty() && "Name cannot be empty.");

        Scene *scene = scenes[name];
        if (scene)
        {
            auto it = scenes.find(name);
            scenes.erase(it);
            CE_CORE_TRACE("Deleting scene '%s', scene count is now %i.", name.c_str(), scenes.size());
            scene->Destroy();
            delete scene;
            scene = nullptr;
        }
    }

    void World::InitActive()
    {
        CE_PROFILE_FUNCTION();

        if (activeScene)
            activeScene->Init();
    }

    void World::StartActive()
    {
        CE_PROFILE_FUNCTION();

        if (activeScene)
            activeScene->Start();
    }

    void World::UpdateActive()
    {
        CE_PROFILE_FUNCTION();

        if (activeScene)
            activeScene->Update();
    }

    void World::RenderActive()
    {
        CE_PROFILE_FUNCTION();

        if (activeScene)
            activeScene->Render();
    }

    void World::StopActive()
    {
        CE_PROFILE_FUNCTION();

        if (activeScene)
            activeScene->Stop();
    }

    void World::DestroyActive()
    {
        CE_PROFILE_FUNCTION();

        if (activeScene)
        {
            activeScene->Destroy();
            delete activeScene;
        }

        activeScene = nullptr;
    }
}