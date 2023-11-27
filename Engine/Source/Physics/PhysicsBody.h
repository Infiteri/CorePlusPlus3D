#pragma once

#include "Base.h"
#include "AABB.h"
#include "Math/Vectors.h"
#include "Colliders.h"

namespace Core
{
    class Actor;
    struct PhysicsBodyConfiguration
    {
        Vector3 Velocity;
        Vector3 Acceleration;
        Actor *Owner;
        float Mass;
        float Damping;
        float Gravity = 9.8f;
    };

    class CE_API PhysicsBody
    {
    private:
        Actor *Owner;
        Collider *collider;
        PhysicsBodyConfiguration Config;
        Vector3 GravityVector;

    public:
        PhysicsBody();
        ~PhysicsBody();

        void Update();

        void SetupWithConfiguration(PhysicsBodyConfiguration Config);

        inline PhysicsBodyConfiguration *GetCurrentConfig() { return &Config; };
        inline Vector3 *GetVelocity() { return &Config.Velocity; };
        inline Vector3 *GetAcceleration() { return &Config.Acceleration; };
        inline Vector3 *GetGravityVector() { return &GravityVector; };
        inline Actor *GetOwner() { return Owner; };
        inline Collider *GetCollider() { return collider; };
        inline float GetMass() { return Config.Mass; };
        inline float GetGravity() { return Config.Gravity; };
        inline void SetMass(float v) { Config.Mass = v; };
        inline void SetGravity(float v) { Config.Gravity = v; };

        void ApplyImpulse(const Vector3 &impulse);
    };
}
