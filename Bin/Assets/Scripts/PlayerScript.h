#pragma once

#include <Core.h>

class PlayerScript : public Core::ActorScript
{
public:
    float moveSpeed = 0.75f;

    PlayerScript();
    ~PlayerScript();

    void OnUpdate();
};

CE_IMPLEMENT_ACTOR_SCRIPT_DEFINE(PlayerScript);
