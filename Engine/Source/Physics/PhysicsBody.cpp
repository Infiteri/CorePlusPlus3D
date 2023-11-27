#include "PhysicsBody.h"
#include "Core/Engine.h"
#include "Core/Logger.h"
#include "Scene/Actor.h"
#include "Math/Math.h"

namespace Core
{
    PhysicsBody::PhysicsBody()
    {
        collider = new AABBCollider();
        Config.Acceleration = Vector3(0.0f, 0.0f, 0.0f);
        GravityVector = Vector3(0.0f, 0.0f, 0.0f);
        Config.Velocity = Vector3(0.0f, 0.0f, 0.0f);
        Config.Mass = 1.0f;
        Config.Gravity = 9.8f;
    }

    PhysicsBody::~PhysicsBody()
    {
    }

    void PhysicsBody::Update()
    {
        auto tr = Owner->GetTransform();
        float dt = Engine::GetDeltaTime();

        tr->position += Config.Velocity * dt;
        Vector3 resultAcceleration = Config.Acceleration;
        resultAcceleration += (Vector3(0, Config.Gravity, 0) * dt);
        Config.Velocity += resultAcceleration * dt;
        Config.Velocity *= Math::Pow(Config.Damping, dt);

        collider->As<AABBCollider>()->GetAABB()->Position = tr->position;
        collider->As<AABBCollider>()->GetAABB()->Sizes = Vector3(1, 1, 1);
    }

    void PhysicsBody::SetupWithConfiguration(PhysicsBodyConfiguration Config)
    {
        this->Config.Velocity = Config.Velocity;
        this->Config.Acceleration = Config.Acceleration; // Set the initial acceleration
        this->Config.Mass = Config.Mass;                 // Set the initial acceleration
        this->Config.Damping = Config.Damping;           // Set the initial acceleration
        this->Config.Gravity = Config.Gravity;           // Set the initial acceleration
        Owner = Config.Owner;

        if (collider)
            delete collider;

        collider = new AABBCollider();
    }

    void PhysicsBody::ApplyImpulse(const Vector3 &impulse)
    {
        // Adjust acceleration based on impulse
        Config.Acceleration += impulse / Config.Mass;
    }
}
