#pragma once

#include <Core.h>
#include <vector>

using namespace Core;

class PlayerScript : public Core::ActorScript
{
public:
    Vector2 velocity;

    float moveSpeed = 0.5f;
    float gravityForce = 0.02f;
    float friction = 0.8f;

    float maxGravity = 1.0f;
    float maxXSpeed = 5.0f;

    AABBComponent *aabb;
    std::vector<AABBComponent *> worldAABBs;

    PlayerScript(){};
    ~PlayerScript(){};

    void OnStart();
    void OnUpdate();

    void Collision();
    void CollisionX();

    void UpdateGravity();
    void UpdateInputs();
};

CE_DEFINE_ACTOR_SCRIPT(PlayerScript);
