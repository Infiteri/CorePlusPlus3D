#include "RigidBody.h"
#include "Core/Engine.h"
#include "Core/Logger.h"
#include "Scene/Actor.h"
#include "Math/Math.h"
#include "Physics/PhysicsEngine.h"

namespace Core
{
    RigidBody::RigidBody()
    {
        collider = new AABBCollider();
        Config.Acceleration = Vector3(0.0f, 0.0f, 0.0f);
        Config.Velocity = Vector3(0.0f, 0.0f, 0.0f);
        Config.Mass = 1.0f;
    }

    RigidBody::~RigidBody()
    {
    }

    void RigidBody::Update()
    {
        auto tr = Owner->GetTransform();
        float dt = Engine::GetDeltaTime();

        tr->position += Config.Velocity * dt;

        Vector3 resultingAcc = CalculateForcesAccumulated();
        resultingAcc.y -= PhysicsConstants::GRAVITY * Config.Mass;

        Config.Velocity += resultingAcc * dt;
        Config.Velocity *= Math::Pow(Config.Damping, dt);

        collider->UpdateFromBody(this);
        forceAccum.Set(0, 0, 0);
    }

    Vector3 RigidBody::CalculateForcesAccumulated()
    {
        Vector3 v;
        v += forceAccum;
        v += Config.Acceleration;
        return v;
    }

    void RigidBody::SetupWithConfiguration(RigidBodyConfiguration Config)
    {
        this->Config.Velocity = {Config.Velocity};

        this->Config.Acceleration = {Config.Acceleration};
        this->Config.Size = {Config.Size};
        this->Config.Mass = Config.Mass;
        this->Config.Damping = Config.Damping;
        Owner = Config.Owner;

        if (collider)
            delete collider;

        collider = new AABBCollider();
    }

    void RigidBody::ApplyImpulse(const Vector3 &v)
    {
        forceAccum += v;
    }
}