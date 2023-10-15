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

        transform.GetRotation()->y += 0.01;
    }

    void Actor::Render()
    {
        if (state == ActorState::Created || state == ActorState::Destroyed) // Rendering can be done in most states
            return;

        Shader *shd = ShaderSystem::Get("EngineResources/Shaders/Object");
        shd->Mat4(transform.GetMatrix(), "uTransform");

        for (Component *component : components)
            component->Render();
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