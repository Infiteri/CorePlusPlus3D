#pragma once

#include <Core.h>

using namespace Core;

class PlayerScript : public Core::ActorScript
{
public:
    float Speed = 3.0f;
    float RotationSpeed = 1.0f;
    float Sensitivity = 0.001f;

    Actor *CameraActor;

public:
    PlayerScript(){};
    ~PlayerScript(){};

    void OnStart();
    void OnUpdate();
};

CE_DEFINE_ACTOR_SCRIPT(PlayerScript);
