#pragma once

#include "Base.h"
#include "Body/RigidBody.h"

#include <vector>

namespace Core
{
    struct ForceGenerator
    {
        virtual void Implement(PhysicsBody *body, float dt) = 0;
    };

    class ForceRegistry
    {
    private:
        std::vector<ForceGenerator *> generators;

    public:
        ForceRegistry();
        ~ForceRegistry();

        void AddGenerator(ForceGenerator *generator);
        void UpdateBodyWithAllGenerators(PhysicsBody *body);
    };
}