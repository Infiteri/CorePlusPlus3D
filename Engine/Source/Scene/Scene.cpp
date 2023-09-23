#include "Scene.h"

#include "Core/Logger.h"

namespace Core
{
    Scene::Scene()
    {
        name = "";

        state = SceneState::Created;
    }

    Scene::~Scene()
    {
        Destroy();
    }

    void Scene::Init()
    {
        state = SceneState::Init;

        for (Actor *a : actors)
            a->Init();
    }

    void Scene::Destroy()
    {
        if (state == SceneState::Destroyed)
            return;

        state = SceneState::Destroyed;

        for (Actor *a : actors)
            delete a;

        actors.clear();
    }

    void Scene::Update()
    {
        if (state != SceneState::Running)
            return;

        for (Actor *a : actors)
            a->Update();
    }

    void Scene::Render()
    {
        if (state != SceneState::Running)
            return;

        for (Actor *a : actors)
            a->Render();
    }

    void Scene::Start()
    {
        state = SceneState::Running;

        for (Actor *a : actors)
            a->Start();
    }

    void Scene::Stop()
    {
        state = SceneState::Stopped;

        for (Actor *a : actors)
            a->Stop();
    }

    void Scene::AddActor(Actor *a)
    {
        if (state == SceneState::Running)
        {
            a->Init();
            a->Start();
        }
        else if (state == SceneState::Init)
        {
            a->Init();
        }

        actors.push_back(a);
    }

    void Scene::SetName(const char *_name)
    {
        name = _name;
    }
}