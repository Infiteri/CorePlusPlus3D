#include "PhysicsBody.h"
#include "Core/Engine.h"
#include "Core/Logger.h"
#include "Scene/Actor.h"
#include "Math/Math.h"

namespace Core
{

    PhysicsBody::PhysicsBody()
    {
        velocity = Vector3(0.0f, 0.0f, 0.0f);
        type = Static;
        collider = new AABBCollider();
        Config.Acceleration = Vector3(0.0f, 0.0f, 0.0f);
        GravityVector = Vector3(0.0f, 0.0f, 0.0f);
        Config.Mass = 1.0f;
    }

    PhysicsBody::~PhysicsBody()
    {
        delete collider;
    }

    void PhysicsBody::Update()
    {
        switch (type)
        {
        case Static:
        default:
            UpdateStatic();
            break;

        case Rigid:
            UpdateRigid();
            break;
        }
    }

    void PhysicsBody::SetupWithConfiguration(PhysicsBodyConfiguration Config)
    {
        this->Config.Acceleration = Config.Acceleration; // Set the initial acceleration
        this->Config.Mass = Config.Mass;                 // Set the initial acceleration
        this->Config.Damping = Config.Damping;           // Set the initial acceleration
        Owner = Config.Owner;
        type = (PhysicsBody::Types)Config.Type;

        if (collider)
            delete collider;

        collider = new AABBCollider();
    }

    void PhysicsBody::ApplyImpulse(const Vector3 &impulse)
    {
        // Adjust acceleration based on impulse
        Config.Acceleration += impulse / Config.Mass;
    }

    void PhysicsBody::UpdateStatic()
    {
    }

    void PhysicsBody::UpdateRigid()
    {
        auto tr = Owner->GetTransform();
        float dt = Engine::GetDeltaTime();

        tr->position += velocity * dt;

        Vector3 resultAcceleration = Config.Acceleration;
        resultAcceleration += (Vector3(0, 9.82f, 0) * dt);

        velocity += resultAcceleration * dt;
        velocity *= Math::Pow(Config.Damping, dt);

        collider->As<AABBCollider>()->GetAABB()->Position = tr->position;
        collider->As<AABBCollider>()->GetAABB()->Sizes = Vector3(1, 1, 1);
    }
}
