#include "Colliders.h"
#include "Physics/Body/RigidBody.h"
#include "Scene/Actor.h"

namespace Core
{
    Collider::Collider()
    {
    }

    Collider::~Collider()
    {
    }

    bool Collider::Intersects(Collider *Other)
    {
        if (this->Type == TypeAABB)
        {
            if (Other->Type == TypeAABB)
            {
                AABBCollider *a = (AABBCollider *)Other;
                AABBCollider *b = (AABBCollider *)this;

                return a->GetAABB()->Collides(b->GetAABB());
            }
        }

        return false;
    }

    AABBCollider::AABBCollider()
    {
        Type = Collider::TypeAABB;
    }

    AABBCollider::~AABBCollider()
    {
    }

    void AABBCollider::UpdateFromBody(RigidBody *b)
    {
        aabb.Position.x = b->GetOwner()->GetWorldMatrix()->data[12];
        aabb.Position.y = b->GetOwner()->GetWorldMatrix()->data[13];
        aabb.Position.z = b->GetOwner()->GetWorldMatrix()->data[14];
        aabb.Sizes *= b->GetOwner()->GetTransform()->scale;
        aabb.TransformMatrix = b->GetOwner()->GetWorldMatrix();
    }

}