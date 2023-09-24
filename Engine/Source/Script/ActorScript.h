#pragma once

#include "Base.h"

#include "Scene/Actor.h"

namespace Core
{
    class CE_API ActorScript
    {
    public:
        Actor *owner;

        ActorScript();
        ~ActorScript();

        virtual void OnStart();
        virtual void OnUpdate();
        virtual void OnStop();
    };
}