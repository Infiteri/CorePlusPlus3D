#include "Colliders.h"

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

}