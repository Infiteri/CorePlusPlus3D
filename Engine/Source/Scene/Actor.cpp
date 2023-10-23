#include "Actor.h"

#include "Core/Logger.h"

#include "Renderer/Objects/Mesh.h"
#include "Renderer/Geometry/BoxGeometry.h"
#include "Renderer/ShaderSystem.h"

namespace Core
{
    static CeU32 GID = 0;

    Actor::Actor()
    {
        state = ActorState::Created;
        name = "Actor";

        id = GID;
        GID++;
    }

    Actor::~Actor()
    {
        id = 0;
        GID--;
    }

    Actor *Actor::From(Actor *other)
    {
        Actor *outActor = new Actor();
        outActor->SetName(other->GetName());

        auto transform = other->GetTransform();
        auto mesh = other->GetComponent<MeshComponent>();
        auto script = other->GetComponent<ActorScriptComponent>();
        auto camera = other->GetComponent<PerspectiveCameraComponent>();
        auto aabb = other->GetComponent<AABBComponent>();

        outActor->GetTransform()->From(transform);

        if (mesh)
            outActor->AddComponent<MeshComponent>()->From(mesh);

        if (script)
            outActor->AddComponent<ActorScriptComponent>()->From(script);

        if (camera)
            outActor->AddComponent<PerspectiveCameraComponent>()->From(camera);

        if (aabb)
            outActor->AddComponent<AABBComponent>()->From(aabb);

        return outActor;
    }

    void Actor::Init()
    {
        state = ActorState::Init;

        for (Component *component : components)
            component->Init();
    }

    void Actor::Destroy()
    {
        if (state != ActorState::Stopped)
            Stop();

        state = ActorState::Destroyed;

        for (Component *component : components)
        {
            component->Destroy();
            delete component;
        }

        components.clear();
    }

    void Actor::Update()
    {
        if (state != ActorState::Started)
            return;

        auto aabb = GetComponent<AABBComponent>();
        if (aabb)
        {
            aabb->x = transform.GetPosition()->x;
            aabb->y = transform.GetPosition()->y;
            aabb->z = transform.GetPosition()->z;
        }
    }

    void Actor::Render()
    {
        if (state == ActorState::Created || state == ActorState::Destroyed) // Rendering can be done in most states
            return;

        Shader *shd = ShaderSystem::Get("EngineResources/Shaders/Object");
        shd->Mat4(transform.GetMatrix(), "uTransform");

        for (Component *component : components)
        {
            component->Render();
        }
    }

    void Actor::Start()
    {
        if (state == ActorState::Created)
            Init();

        state = ActorState::Started;

        for (Component *component : components)
            component->Start();
    }

    void Actor::Stop()
    {
        state = ActorState::Stopped;

        for (Component *component : components)
        {
            component->Stop();
        }
    }

    void Actor::SetName(const std::string &name)
    {
        this->name = name;
    }

    std::string Actor::GetName()
    {
        return name;
    }

}