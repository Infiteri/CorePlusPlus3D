#include "Actor.h"

#include "Core/Logger.h"

#include "Renderer/Objects/Mesh.h"
#include "Renderer/Geometry/BoxGeometry.h"

namespace Core
{
    Actor::Actor()
    {
        state = ActorState::Created;
        name = "Actor";
    }

    Actor::~Actor()
    {
    }

    void Actor::Init()
    {
        state = ActorState::Init;

        for (Component *component : components)
        {
            component->Init();
        }
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
    }

    void Actor::Render()
    {
        if (state != ActorState::Started)
            return;

        for (Component *component : components)
        {
            component->Render();
        }
    }

    void Actor::Start()
    {
        state = ActorState::Started;

        for (Component *component : components)
        {
            component->Start();
        }
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