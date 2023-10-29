#include "Actor.h"

#include "Core/Logger.h"

#include "Renderer/Objects/Mesh.h"
#include "Renderer/Geometry/BoxGeometry.h"
#include "Renderer/ShaderSystem.h"

#include <algorithm>

namespace Core
{

    Actor::Actor()
    {
        state = ActorState::Created;
        name = "Actor";
        parent = nullptr;

        CE_TRACE("Made actor: %i", uuid.Get());
    }

    Actor::~Actor()
    {
    }

    Actor *Actor::From(Actor *other)
    {
        if (!other)
            return nullptr;

        Actor *outActor = new Actor();
        outActor->SetName(other->GetName());
        outActor->SetParent(other->GetParent());

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

        for (Actor *a : other->GetChildren())
        {
            outActor->AddChild(Actor::From(a));
        }

        return outActor;
    }

    void Actor::Init()
    {
        state = ActorState::Init;

        for (Component *component : components)
            component->Init();

        for (Actor *a : children)
        {
            a->Init();
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

        for (Actor *a : children)
        {
            a->Destroy();
            delete a;
        }

        components.clear();
        children.clear();
    }

    void Actor::Update()
    {
        if (state != ActorState::Started)
            return;

        auto aabb = GetComponent<AABBComponent>();
        if (aabb)
        {
            aabb->x = transform.position.x;
            aabb->y = transform.position.y;
            aabb->z = transform.position.z;
        }

        for (Actor *a : children)
        {
            a->Update();
        }
    }

    void Actor::Render()
    {
        if (state == ActorState::Created || state == ActorState::Destroyed) // Rendering can be done in most states
            return;

        CalculateMatrices();
        Shader *shd = ShaderSystem::Get("EngineResources/Shaders/Object");
        shd->Mat4(worldMatrix, "uTransform");

        for (Component *component : components)
        {
            if (component)
            {
                component->Render();
            }
        }

        for (Actor *a : children)
        {
            a->Render();
        }
    }

    void Actor::Start()
    {
        if (state == ActorState::Created)
            Init();

        state = ActorState::Started;

        for (Component *component : components)
            component->Start();

        for (Actor *a : children)
        {
            a->Start();
        }
    }

    void Actor::Stop()
    {
        state = ActorState::Stopped;

        for (Component *component : components)
        {
            component->Stop();
        }

        for (Actor *a : children)
        {
            a->Stop();
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

    void Actor::SetParent(Actor *a)
    {
        parent = a;
    }

    void Actor::AddChild(Actor *actor)
    {
        actor->parent = this;
        children.push_back(actor);

        if (state == ActorState::Init)
        {
            actor->Init();
        }
        else if (state == ActorState::Started)
        {
            actor->Start();
        }
    }

    Actor *Actor::GetChildByName(const std::string &name)
    {
        // TODO: Hierarchy search.
        for (Actor *a : children)
        {
            if (a->GetName() == name)
                return a;
        }

        return nullptr;
    }

    void Actor::RemoveChildByName(const std::string &name)
    {
        // TODO: Hierarchy search.
        auto it = std::remove_if(children.begin(), children.end(), [&name](Actor *actor)
                                 { return actor->GetName() == name; });

        if (it != children.end())
        {
            // Element with the specified name found, erase it from the vector
            children.erase(it, children.end());
        }
    }

    void Actor::RemoveChildUUID(UUID *uid)
    {
        for (int i = 0; i < children.size(); i++)
        {
            if (children[i]->GetUUID()->Get() == uid->Get())
            {
                children[i]->Destroy();
                delete children[i];

                //? Remove vector
                auto it = children.begin();
                std::advance(it, i);
                children.erase(it);
            }
        }
    }

    void Actor::SetUUID(CeU64 uid)
    {
        uuid = uid;
    };

    void Actor::CalculateMatrices()
    {
        localMatrix = transform.GetMatrix();

        //? Calculate the world matrix with the parent matrix if its parented
        if (parent)
        {
            worldMatrix = Matrix4::Multiply(&parent->worldMatrix, &localMatrix);
        }
        else
        {
            worldMatrix = localMatrix;
        }
    }
}