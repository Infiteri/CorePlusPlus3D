#include "Scene.h"
#include "Core/Logger.h"
#include "Core/Engine.h"

#include "Math/Math.h"

#include "Renderer/Camera/CameraSystem.h"
#include "Renderer/Geometry/BoxGeometry.h"
#include "Renderer/Geometry/PlaneGeometry.h"

#include "Script/ScriptEngine.h"

namespace Core
{
    SceneEnvironment::~SceneEnvironment()
    {
        delete directionalLight;
        delete sky;
    }

    Scene::Scene()
    {
        name = "Scene";

        environment.Raw()->directionalLight = new DirectionalLight();
        environment.Raw()->sky = new Sky();

        state = SceneState::Created;
    }

    Scene::~Scene()
    {
    }

    Scene *Scene::GetCopyOfScene(Scene *other)
    {
        Scene *scene = new Scene();

        scene->SetName(other->GetName());

        auto nDirLight = scene->GetEnvironment()->directionalLight;
        auto oDirLight = other->GetEnvironment()->directionalLight;
        nDirLight->GetColor()->Set(oDirLight->GetColor());
        nDirLight->GetTransform()->From(oDirLight->GetTransform());
        nDirLight->GetDirection()->Set(oDirLight->GetDirection());

        auto nSky = scene->GetEnvironment()->sky;
        auto oSky = other->GetEnvironment()->sky;
        nSky->SetMode(oSky->GetMode());
        nSky->From(oSky);

        for (Actor *a : other->GetActors())
        {
            Actor *outActor = new Actor();
            outActor->SetName(a->GetName());

            auto transform = a->GetTransform();
            auto mesh = a->GetComponent<MeshComponent>();
            auto script = a->GetComponent<ActorScriptComponent>();
            auto camera = a->GetComponent<PerspectiveCameraComponent>();

            outActor->GetTransform()->From(transform);

            if (mesh)
                outActor->AddComponent<MeshComponent>()->From(mesh);

            if (script)
                outActor->AddComponent<ActorScriptComponent>()->From(script);

            if (camera)
                outActor->AddComponent<PerspectiveCameraComponent>()->From(camera);

            scene->AddActor(outActor);
        }

        return scene;
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
        {
            a->Destroy();
            delete a;
        }
        actors.clear();
    }

    void Scene::Update()
    {
        if (state != SceneState::Running)
            return;

        for (Actor *a : actors)
            a->Update();

        ScriptEngine::UpdateRuntime();
    }

    void Scene::Render()
    {
        if (state == SceneState::Created) // Rendering can be done in most states
            return;

        if (state == SceneState::Destroyed)
            return;

        environment.Raw()->directionalLight->Update();

        for (Actor *a : actors)
            a->Render();
    }

    void Scene::Start()
    {
        state = SceneState::Running;

        for (Actor *a : actors)
        {
            a->Start();

            // Register actor script
            auto componentScript = a->GetComponent<ActorScriptComponent>();
            if (componentScript != nullptr)
            {
                ScriptEngine::RegisterScript(componentScript->className, componentScript->className, a);
            }
        }

        ScriptEngine::StartRuntime();
    }

    void Scene::Stop()
    {
        state = SceneState::Stopped;

        for (Actor *a : actors)
            a->Stop();

        ScriptEngine::StopRuntime();
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

    void Scene::RemoveActorByName(const std::string &name)
    {
        for (Actor *a : actors)
        {
            if (a->GetName() == name)
                RemoveActorByID(a->GetID());
        }
    }

    void Scene::RemoveActorByID(CeU32 id)
    {
        auto it = actors.begin();

        // Iterate over the vector
        while (it != actors.end())
        {
            if ((*it)->GetID() == id)
            {
                delete *it;
                it = actors.erase(it);
            }
            else
                ++it;
        }
    }

    void Scene::ActivateSceneCamera()
    {
        auto comp = GetActorCameraComponent();

        if (comp)
            CameraSystem::Activate(comp->camera);
        else
            CameraSystem::SetActiveCameraToNone();
    }

    PerspectiveCameraComponent *Scene::GetActorCameraComponent()
    {
        for (Actor *a : actors)
        {
            auto comp = a->GetComponent<PerspectiveCameraComponent>();
            if (comp)
            {
                comp->camera->GetPosition()->Set(a->GetTransform()->GetPosition());
                comp->camera->GetRotation()->Set(a->GetTransform()->GetRotation());
                return comp;
            }
        }

        return nullptr;
    }

    void Scene::SetName(const std::string &_name)
    {
        name = _name;
    }

    SceneEnvironment *Scene::GetEnvironment()
    {
        return environment.Raw();
    }

    DirectionalLight *Scene::GetEnvironmentDirectionalLight()
    {
        return environment.Raw()->directionalLight;
    }

    std::vector<Actor *> Scene::GetActors()
    {
        return actors;
    }
}