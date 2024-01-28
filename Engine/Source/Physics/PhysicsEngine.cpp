#include "PhysicsEngine.h"
#include "Core/Logger.h"
#include "Scene/Actor.h"
#include "Core/Engine.h"

namespace Core
{
    static PhysicsEngineState State;

    const float PhysicsConstants::GRAVITY = 9.82f;

    void PhysicsEngine::Init()
    {
        State.Stage = StageInit;
    }

    RigidBody *PhysicsEngine::CreateRigidBody(RigidBodyConfiguration Config)

    {
        RigidBody *body = new RigidBody();
        body->SetupWithConfiguration(Config);
        State.Bodies.push_back(body);
        return body;
    }

    void PhysicsEngine::UpdateRuntime()
    {
        CE_PROFILE_FUNCTION();

        State.Stage = StageUpdating;

        for (RigidBody *body : State.Bodies)
        {
            body->Update();
            State.Forces.UpdateBodyWithAllGenerators(body);
        }

        UpdateCollision();
    }

    void PhysicsEngine::UpdateCollision()
    {
        CE_PROFILE_FUNCTION();
        // for (int i = 0; i < State.Bodies.size(); i++)
        // {
        //     for (int j = i + 1; j < State.Bodies.size(); j++)
        //     {
        //         auto a = State.Bodies[i];
        //         auto b = State.Bodies[j];
        //     }
        // }
    }

    void PhysicsEngine::StopRuntime()
    {
        if (State.Stage != StageUpdating)
            return;

        State.Stage = StageStop;
        PhysicsEngine::ClearBodies();
    }

    void PhysicsEngine::ClearBodies()
    {
        for (auto b : State.Bodies)
        {
            delete b;
        }

        State.Bodies.clear();
    }

    void PhysicsEngine::Shutdown()
    {
        State.Stage = StageShutdown;
        ClearBodies();
    }
}