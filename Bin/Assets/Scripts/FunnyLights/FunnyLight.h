#pragma once

#include <Core.h>

class FunnyLight : public Core::ActorScript {
public:

    FunnyLight() {};
    ~FunnyLight() {};

    void OnStart();
    void OnUpdate();
};

CE_DEFINE_ACTOR_SCRIPT(FunnyLight);
