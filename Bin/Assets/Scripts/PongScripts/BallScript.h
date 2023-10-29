#pragma once

#include <Core.h>

class BallScript : public Core::ActorScript
{
public:
    Core::Vector2 velocity;

    Core::AABBComponent* aabb;
    Core::AABBComponent* paddleLeft;
    Core::AABBComponent* paddleRight;
    Core::AABBComponent* left;
    Core::AABBComponent* right;
    Core::AABBComponent* top;
    Core::AABBComponent* bottom;

    BallScript(){};
    ~BallScript(){};

    void OnStart();
    void OnUpdate();
};

CE_DEFINE_ACTOR_SCRIPT(BallScript);
