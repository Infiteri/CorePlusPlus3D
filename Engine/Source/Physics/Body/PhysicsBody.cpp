#include "PhysicsBody.h"

namespace Core
{
    PhysicsBody::PhysicsBody()
    {
        collider = new AABBCollider();
    }

    PhysicsBody::~PhysicsBody()
    {
    }

    void PhysicsBody::Update()
    {
    }
}