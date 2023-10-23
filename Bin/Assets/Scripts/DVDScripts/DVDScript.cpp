#include "DVDScript.h"

DVDScript::DVDScript() {}

DVDScript::~DVDScript() {}

void DVDScript::OnStart()
{
    aabb = GetComponent<AABBComponent>();

    left = GetActorByName("Wall1")->GetComponent<AABBComponent>();
    right = GetActorByName("Wall2")->GetComponent<AABBComponent>();
    top = GetActorByName("Wall3")->GetComponent<AABBComponent>();
    bottom = GetActorByName("Wall4")->GetComponent<AABBComponent>();

    velocity = Vector2(0.2, 0.2);
}

void DVDScript::OnUpdate()
{
    if (!aabb || !left || !right || !top || !bottom)
    {
        return;
    }

    aabb->padding.x = velocity.x;
    aabb->padding.y = velocity.y;

    if (aabb->CollidesWith(left))
    {
        velocity.x = -velocity.x;
    }

    if (aabb->CollidesWith(right))
    {
        velocity.x = -velocity.x;
    }

    if (aabb->CollidesWith(top))
    {
        velocity.y = -velocity.y;
    }

    if (aabb->CollidesWith(bottom))
    {
        velocity.y = -velocity.y;
    }

    GetTransform()->GetPosition()->x += velocity.x;
    GetTransform()->GetPosition()->y += velocity.y;
}

CE_IMPLEMENT_ACTOR_SCRIPT_DEFINE(DVDScript);
