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
        Vector3 Acceleration = {0, 0, 0};
        Actor *Owner = nullptr;
        float Mass = 1.0f;
        float Damping = 1.0f;
        enum Types
        {
            Static,
            Rigid
        } Type = Static;
    };

    class CE_API PhysicsBody
    {
    public:
        enum Types
        {
            Static,
            Rigid
        };

    private:
        Types type;
        Actor *Owner;
        Collider *collider;
        PhysicsBodyConfiguration Config;
        Vector3 GravityVector;
        Vector3 velocity;

        void UpdateStatic();
        void UpdateRigid();

    public:
        PhysicsBody();
        ~PhysicsBody();

        void Update();

        void SetupWithConfiguration(PhysicsBodyConfiguration Config);

        inline PhysicsBodyConfiguration *GetCurrentConfig() { return &Config; };
        inline Vector3 *GetVelocity() { return &velocity; };
        inline Vector3 *GetAcceleration() { return &Config.Acceleration; };
        inline Vector3 *GetGravityVector() { return &GravityVector; };
        inline Actor *GetOwner() { return Owner; };
        inline Collider *GetCollider() { return collider; };

        inline float GetMass() { return Config.Mass; };
        inline void SetMass(float v) { Config.Mass = v; };

        inline Types GetType() { return type; };
        inline void SetType(Types v) { type = v; };

        void ApplyImpulse(const Vector3 &impulse);
    };
}
