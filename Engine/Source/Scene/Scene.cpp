#include "Scene.h"
#include "Core/Logger.h"
#include "Core/Engine.h"

#include "Math/Math.h"

#include "Renderer/Camera/CameraSystem.h"
#include "Renderer/Geometry/BoxGeometry.h"
#include "Renderer/Geometry/PlaneGeometry.h"
#include "Renderer/ShaderSystem.h"

#include "Script/ScriptEngine.h"

#include "Physics/PhysicsEngine.h"

#include <algorithm>

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
        CE_PROFILE_FUNCTION();

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
            Actor *outActor = Actor::From(a);
            scene->AddActor(outActor);
        }

        return scene;
    }

    void Scene::Init()
    {
        CE_PROFILE_FUNCTION();

        state = SceneState::Init;

        for (Actor *a : actors)
            a->Init();
    }

    void Scene::Destroy()
    {
        CE_PROFILE_FUNCTION();

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
        CE_PROFILE_FUNCTION();

        if (state != SceneState::Running)
            return;

        for (Actor *a : actors)
            a->Update();

        // Runtime updates
        PhysicsEngine::UpdateRuntime();
        ScriptEngine::UpdateRuntime();
    }

    void Scene::Render()
    {
        CE_PROFILE_FUNCTION();

        if (state == SceneState::Created) // Rendering can be done in most states
            return;

        if (state == SceneState::Destroyed)
            return;

        environment.Raw()->directionalLight->Update();

        for (Actor *a : actors)
        {
            a->Render();
        }

        ShaderSystem::Get("EngineResources/Shaders/Object")->Int(environment.Raw()->pointLightCount, "uPointLightCount");
        ShaderSystem::Get("EngineResources/Shaders/Object")->Int(environment.Raw()->spotLightCount, "uSpotLightCount");

        environment.Raw()->pointLightCount = 0;
        environment.Raw()->spotLightCount = 0;
    }

    void Scene::Start()
    {
        CE_PROFILE_FUNCTION();

        state = SceneState::Running;

        PhysicsEngine::ClearBodies();

        for (Actor *a : actors)
        {
            a->Start();
            AddRuntimeComponents(a);
        }

        ScriptEngine::StartRuntime();
    }

    void Scene::Stop()
    {
        CE_PROFILE_FUNCTION();

        state = SceneState::Stopped;

        for (Actor *a : actors)
            a->Stop();

        PhysicsEngine::StopRuntime();
        ScriptEngine::StopRuntime();
    }

    Actor *Scene::GetActorByName(const std::string &name)
    {
        CE_PROFILE_FUNCTION();

        for (Actor *a : actors)
        {
            if (a->GetName() == name)
                return a;
        }

        return nullptr;
    }

    Actor *Scene::GetActorByUUID(UUID *uid)
    {
        CE_PROFILE_FUNCTION();

        for (Actor *a : actors)
        {
            if (a->GetUUID()->Get() == uid->Get())
                return a;
        }

        return nullptr;
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

        AddRuntimeComponents(a); 

        actors.push_back(a);
    }

    void Scene::RemoveActorByName(const std::string &name)
    {
        CE_PROFILE_FUNCTION();

        for (Actor *a : actors)
        {
            if (a->GetName() == name)
                RemoveActorByUUID(a->GetUUID());
        }
    }

    void Scene::RemoveActorByUUID(UUID *id)
    {
        CE_PROFILE_FUNCTION();

        for (int i = 0; i < actors.size(); i++)
        {
            if (actors[i]->GetUUID()->Get() == id->Get())
            {
                actors[i]->Destroy();
                delete actors[i];

                //? Remove vector
                auto it = actors.begin();
                std::advance(it, i);
                actors.erase(it);
            }
        }
    }

    void Scene::AddRuntimeComponents(Actor *a)
    {
        if (state != SceneState::Running)
            return;

        //? Register actor runtime components
        auto componentScript = a->GetComponent<ActorScriptComponent>();
        if (componentScript != nullptr)
        {
            ScriptEngine::RegisterScript(componentScript->className, componentScript->className, a);
        }

        auto componentPhysics = a->GetComponent<PhysicsComponent>();
        if (componentPhysics != nullptr)
        {
            componentPhysics->Configuration.Owner = a;
            componentPhysics->Body =
                PhysicsEngine::CreatePhysicsBody(componentPhysics->Configuration);
        }
    }

    void Scene::ActivateSceneCamera()
    {
        CE_PROFILE_FUNCTION();

        auto comp = GetActorCameraComponent();

        if (comp)
            CameraSystem::Activate(comp->camera);
        else
            CameraSystem::SetActiveCameraToNone();
    }

    PerspectiveCameraComponent *Scene::GetActorCameraComponent()
    {
        CE_PROFILE_FUNCTION();

        bool found = false;

        for (Actor *a : actors)
        {
            auto comp = a->GetComponent<PerspectiveCameraComponent>();
            if (comp)
            {
                found = true;
                return comp;
            }
        }
        if (!found)
        {
            for (Actor *a : actors)
            {
                auto comp = a->SearchChildrenForComponent<PerspectiveCameraComponent>();
                if (comp)
                    return comp;
            }
        }

        return nullptr;
    }

    void Scene::SetName(const std::string &_name)
    {
        name = _name;
    }

    void Scene::MoveActorInHierarchy(const std::string &name, int newIndex)
    {
        CE_PROFILE_FUNCTION();

        // Find the actor by name
        Actor *actorToMove = GetActorByName(name);

        if (!actorToMove)
        {
            return;
        }

        if (newIndex < 0 || newIndex >= actors.size())
        {
            return;
        }

        // Find the current index of the actor
        auto actorIterator = std::find(actors.begin(), actors.end(), actorToMove);

        if (actorIterator != actors.end())
        {
            size_t currentIndex = std::distance(actors.begin(), actorIterator);

            // Remove the actor from the current position
            actors.erase(actorIterator);

            // Insert the actor at the new index
            actors.insert(actors.begin() + newIndex, actorToMove);
        }
    }

    void Scene::MoveActorInHierarchy(UUID *uid, int newIndex)
    {
        CE_PROFILE_FUNCTION();

        Actor *actorToMove = GetActorByUUID(uid);

        if (!actorToMove)
        {
            return;
        }

        if (newIndex < 0 || newIndex >= actors.size())
        {
            return;
        }

        // Find the current index of the actor
        auto actorIterator = std::find(actors.begin(), actors.end(), actorToMove);

        if (actorIterator != actors.end())
        {
            size_t currentIndex = std::distance(actors.begin(), actorIterator);

            // Remove the actor from the current position
            actors.erase(actorIterator);

            // Insert the actor at the new index
            actors.insert(actors.begin() + newIndex, actorToMove);
        }
    }

    int Scene::GetActorCount()
    {
        return actors.size();
    }

    Actor *Scene::GetActorAtIndex(int i)
    {
        CE_PROFILE_FUNCTION();

        if (i > actors.size())
        {
            CE_CORE_ERROR("Unable to get actor at index %i, out of bounds. (Actor count is %i.)", i, actors.size());
            return nullptr;
        }

        return actors[i];
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