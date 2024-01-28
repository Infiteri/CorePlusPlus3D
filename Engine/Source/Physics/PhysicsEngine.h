#pragma once

#include "Base.h"
#include "Physics/Body/BodyConfigurations.h"
#include "ForceGenerator.h"
#include "Math/Vectors.h"
#include <vector>

namespace Core
{
    struct PhysicsEngineCollisionContact;

    struct PhysicsConstants
    {
        static const float GRAVITY;
    };

    class CE_API PhysicsEngine
    {
    public:
        PhysicsEngine(){};
        ~PhysicsEngine(){};

        static void Init();
        static RigidBody *CreateRigidBody(RigidBodyConfiguration Config);

        /// @brief Called when the scene is updating, will do the maths.
        static void UpdateRuntime();
        static void UpdateCollision();
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
        std::vector<RigidBody *> Bodies;
        ForceRegistry Forces;
    };
}
