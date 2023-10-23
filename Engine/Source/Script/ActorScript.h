#pragma once

#include "Base.h"

#include "Scene/Actor.h"

namespace Core
{
    class CE_API ActorScript
    {
    public:
        Actor *owner = nullptr;

        ActorScript();
        ~ActorScript();

        virtual void OnStart();
        virtual void OnUpdate();
        virtual void OnStop();

        Transform *GetTransform();

        Actor *GetActorByName(const std::string &name);

        template <typename T>
        T *GetComponent()
        {
            if (owner == nullptr)
                return nullptr;
            return owner->GetComponent<T>();
        }

        template <typename T>
        T *AddComponent()
        {
            if (owner == nullptr)
                return nullptr;
            return owner->AddComponent<T>();
        }

        template <typename T>
        bool HasComponent()
        {
            if (owner == nullptr)
                return nullptr;
            return owner->HasComponent<T>();
        }
    };
}