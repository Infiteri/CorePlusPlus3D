#include "BallScript.h"

void BallScript::OnStart()
{
    velocity.Set(-1, 0);

    aabb = GetComponent<Core::AABBComponent>();

    auto wallContainer = GetActorByName("Walls");
    left = wallContainer->GetChildByName("Left")->GetComponent<Core::AABBComponent>();
    right = wallContainer->GetChildByName("Right")->GetComponent<Core::AABBComponent>();
    top = wallContainer->GetChildByName("Top")->GetComponent<Core::AABBComponent>();
    bottom = wallContainer->GetChildByName("Bottom")->GetComponent<Core::AABBComponent>();

    paddleLeft = GetActorByName("Left")->GetComponent<Core::AABBComponent>();
    paddleRight = GetActorByName("Right")->GetComponent<Core::AABBComponent>();
}

void BallScript::OnUpdate()
{
    auto transform = GetTransform();

    if (aabb->CollidesWith(paddleLeft) || aabb->CollidesWith(paddleRight))
    {
        velocity.y = Core::Math::RandomFloat(-.9, .9);
        velocity.x = -velocity.x;
    }

    if (aabb->CollidesWith(left) || aabb->CollidesWith(right))
    {
        // TODO: Game loss
        velocity.x = -velocity.x;
    }

    if (aabb->CollidesWith(top) || aabb->CollidesWith(bottom))
    {
        velocity.y = -velocity.y;
    }

    transform->position.x += velocity.x;
    transform->position.y += velocity.y;
}

CE_IMPLEMENT_ACTOR_SCRIPT_DEFINE(BallScript);
