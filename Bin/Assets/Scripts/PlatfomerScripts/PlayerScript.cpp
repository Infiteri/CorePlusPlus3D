#include "PlayerScript.h"

void PlayerScript::OnStart()
{
    aabb = GetComponent<AABBComponent>();

    worldAABBs.push_back(GetActorByName("Floor")->GetComponent<AABBComponent>());
    worldAABBs.push_back(GetActorByName("Wall")->GetComponent<AABBComponent>());
}

void PlayerScript::OnUpdate()
{
    auto transform = GetTransform();
    auto pos = transform->position;

    Collision();
    transform->position.x += velocity.x;
    transform->position.y += velocity.y;

    UpdateGravity();
    UpdateInputs();
}

void PlayerScript::Collision()
{
    auto transform = GetTransform();
    auto pos = transform->position;

    for (AABBComponent *a : worldAABBs)
    {
        if (aabb->CollidesWith(a))
        {
            if (velocity.x < 0)
            {
                pos.x = a->x + a->width + 0.01;
                velocity.x *= 0;
            }

            if (velocity.x > 0)
            {
                pos.x = a->x - a->width - 0.01;
                velocity.x *= 0;
            }

            if (velocity.y < 0)
            {
                pos.y = a->y + a->height + 0.01;
                velocity.y *= 0;
            }

            if (velocity.y > 0)
            {
                pos.y = a->y - a->height - 0.01;
                velocity.y *= 0;
            }
        }
    }
}

void PlayerScript::CollisionX()
{
    auto transform = GetTransform();
    auto pos = transform->position;

    for (AABBComponent *a : worldAABBs)
    {
        if (aabb->CollidesWith(a))
        {
            if (velocity.x < 0.01)
            {
                pos.x = a->x + a->width + 0.01;
                velocity.x *= 0;
            }

            if (velocity.x > 0.01)
            {
                pos.x = a->x - a->width - 0.01;
                velocity.x *= 0;
            }
        }
    }
}

void PlayerScript::UpdateGravity()
{
    velocity.y -= gravityForce;

    if (velocity.y <= -maxGravity)
        velocity.y = -maxGravity;
}

void PlayerScript::UpdateInputs()
{
    if (Input::GetKey(Keys::A))
    {
        velocity.x -= moveSpeed;
        if (velocity.x < moveSpeed)
            velocity.x = -moveSpeed;
    }

    if (Input::GetKey(Keys::D))
    {
        velocity.x += moveSpeed;
        if (velocity.x > moveSpeed)
            velocity.x = moveSpeed;
    }

    velocity.x *= friction;
}

CE_IMPLEMENT_ACTOR_SCRIPT_DEFINE(PlayerScript);
