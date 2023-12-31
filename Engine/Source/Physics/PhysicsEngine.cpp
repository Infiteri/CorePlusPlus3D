#include "PhysicsEngine.h"
#include "Core/Logger.h"
#include "Scene/Actor.h"

namespace Core
{
    static PhysicsEngineState State;

    void PhysicsEngine::Init()
    {
        State.Stage = StageInit;
    }

    PhysicsBody *PhysicsEngine::CreatePhysicsBody(PhysicsBodyConfiguration Config)
    {
        PhysicsBody *body = new PhysicsBody();
        body->SetupWithConfiguration(Config);
        State.Bodies.push_back(body);
        return body;
    }

    void PhysicsEngine::UpdateRuntime()
    {
        CE_PROFILE_FUNCTION();

        State.Stage = StageUpdating;

        for (PhysicsBody *body : State.Bodies)
        {
            body->Update();
        }

        UpdateCollision();
    }

    void PhysicsEngine::UpdateCollision()
    {
        CE_PROFILE_FUNCTION();
        for (int i = 0; i < State.Bodies.size(); i++)
        {
            for (int j = i + 1; j < State.Bodies.size(); j++)
            {
                auto a = State.Bodies[i];
                auto b = State.Bodies[j];

#if 0
                if (a->GetCollider()->Intersects(b->GetCollider()))
                    ResolveCollision(a, b);
#else
                if (a->GetCollider()->Intersects(b->GetCollider()))
                {
                    PhysicsEngineCollisionContact Contact;
                    Contact.A = a;
                    Contact.B = b;
                    ResolveContact(&Contact);
                }
#endif
            }
        }
    }

    void PhysicsEngine::ResolveCollision(PhysicsBody *a, PhysicsBody *b)
    {
        CE_PROFILE_FUNCTION();

        // Get the colliders of the bodies
        auto colliderA = a->GetCollider()->As<AABBCollider>();
        auto colliderB = b->GetCollider()->As<AABBCollider>();

        a->GetVelocity()->Set(*a->GetVelocity() * -1);
        a->GetGravityVector()->Set(0, 0, 0);
        b->GetVelocity()->Set(*b->GetVelocity() * -1);
        b->GetGravityVector()->Set(0, 0, 0);
    }

    void PhysicsEngine::ResolveContact(PhysicsEngineCollisionContact *c)
    {
        if (!c->A || !c->B)
            return;

        // -- Vars --
        PhysicsBody *a = c->A;
        PhysicsBody *b = c->B;
        // ---------
    
        a->GetVelocity()->Set(*a->GetVelocity() * -1);
        b->GetVelocity()->Set(*b->GetVelocity() * -1);
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