#pragma once

#include "Base.h"
#include "Physics/AABB.h"
#include "PhysicsBody.h"
#include "Math/Vectors.h"
#include "Physics/Colliders.h"
#include "BodyConfigurations.h"

namespace Core
{
    class Actor;

    class CE_API RigidBody : public PhysicsBody
    {
    private:
        RigidBodyConfiguration Config;
        Vector3 forceAccum;

    public:
        RigidBody();
        ~RigidBody();

        void Update();

        void SetupWithConfiguration(RigidBodyConfiguration Config);
        Vector3 CalculateForcesAccumulated();

        inline RigidBodyConfiguration *GetCurrentConfig() { return &Config; };
        inline Vector3 *GetVelocity() { return &Config.Velocity; };
        inline Vector3 *GetAcceleration() { return &Config.Acceleration; };
        inline float GetMass() { return Config.Mass; };
        inline void SetMass(float v) { Config.Mass = v; };

        void ApplyImpulse(const Vector3& v);
    };
}
