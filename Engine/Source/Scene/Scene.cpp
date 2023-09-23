#include "Scene.h"
#include "Core/Logger.h"
#include "Core/Engine.h"

#include "Math/Math.h"

#include "Renderer/Camera/CameraSystem.h"

namespace Core
{
    Scene::Scene()
    {
        name = "Scene";
        sceneCameraName = "__NONE__INVALID__";

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

        //? Set active camera
        ActivateSceneCamera();

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

    void Scene::ActivateSceneCamera()
    {
        if (sceneCameraName.compare("__NONE__INVALID__") == 0)
            return;

        bool cameraExists = CameraSystem::DoesCameraExist(sceneCameraName);
        if (!cameraExists)
        {
            CE_WARN("Camera '%s' doesn't exist. Creating it now.", sceneCameraName.c_str());
            CameraSystem::Generate(sceneCameraName, Math::DegToRad(90), Engine::GetWindowAspect(), 0.01f, 1000.0f);
        }

        CameraSystem::Activate(sceneCameraName);
    }

    void Scene::SetName(const char *_name)
    {
        name = _name;
    }

    void Scene::SetSceneCameraName(const std::string &cameraName)
    {
        if (sceneCameraName == cameraName)
            return;

        sceneCameraName = cameraName;
    }

    std::string Scene::GetSceneCameraName() const
    {
        return sceneCameraName;
    }

    void Scene::GenerateAndActivateSceneCamera(const std::string &cameraName, float fov, float aspect, float near, float far)
    {

        if (CameraSystem::DoesCameraExist(cameraName))
        {
            CE_ERROR("Camera '%s' exists already.", cameraName.c_str());
            return;
        }

        SetSceneCameraName(cameraName);
        CameraSystem::Generate(cameraName, fov, aspect, near, far);
        ActivateSceneCamera();
    }
}