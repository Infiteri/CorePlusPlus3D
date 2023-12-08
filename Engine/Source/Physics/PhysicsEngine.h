#pragma once

#include "Base.h"
#include "PhysicsBody.h"
#include "Math/Vectors.h"
#include <vector>

namespace Core
{
    struct PhysicsEngineCollisionContact;
    class CE_API PhysicsEngine
    {
    public:
        PhysicsEngine(){};
        ~PhysicsEngine(){};

        static void Init();
        static PhysicsBody *CreatePhysicsBody(PhysicsBodyConfiguration Config);

        /// @brief Called when the scene is updating, will do the maths.
        static void UpdateRuntime();
        static void UpdateCollision();
        static void ResolveCollision(PhysicsBody *a, PhysicsBody *b);
        static void ResolveContact(PhysicsEngineCollisionContact *c);
        static void StopRuntime();

        static void ClearBodies();

        static void Shutdown();

    public:
        enum PhysicsEngineStage
        {
            StageInit,
            StageUpdating,
            StageStop,
            StageShutdown,
        };
    };

    struct PhysicsEngineState
    {
        PhysicsEngine::PhysicsEngineStage Stage;
        std::vector<PhysicsBody *> Bodies;
    };

    struct PhysicsEngineCollisionContact
    {
        PhysicsBody *A;
        PhysicsBody *B;
        Vector3 ContactPoint;
        Vector3 ContactNormal;
        float Penetration;
    };
}
