#pragma once

#include <Core.h>

class PlayerScript : public Core::ActorScript
{
public:
    float speed = 1.0f;
    Core::Vector2 velocity;

    PlayerScript(){};
    ~PlayerScript(){};

    void OnUpdate();
};

CE_DEFINE_ACTOR_SCRIPT(PlayerScript);
