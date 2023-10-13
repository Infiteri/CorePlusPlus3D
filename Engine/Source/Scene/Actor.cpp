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
        if (state != ActorState::Started && state != ActorState::Init)
            return;
    }

    void Actor::Render()
    {
        if (state != ActorState::Started && state != ActorState::Init)
        {
            CE_TRACE("%s not right", name.c_str());
            return;
        }

        Shader *shd = ShaderSystem::Get("EngineResources/Shaders/Object");
        transform.GetRotation()->y += 0.01;
        shd->Mat4(transform.GetMatrix(), "uTransform");

        for (Component *component : components)
        {
            component->Render();
        }
    }

    void Actor::Start()
    {
        CE_TRACE("State: %i", state);

        if (state == ActorState::Created)
            Init();

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