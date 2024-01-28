#pragma once

#include "Base.h"
#include "Physics/AABB.h"
#include "Math/Vectors.h"
#include "Physics/Colliders.h"
#include "Scene/Actor.h"

namespace Core
{
    class CE_API PhysicsBody
    {
    protected:
        Actor *Owner;
        Collider *collider;

    public:
        PhysicsBody();
        ~PhysicsBody();

        virtual void Update();
        inline Collider *GetCollider() { return collider; };
        inline Actor *GetOwner() { return Owner; };
    };

}