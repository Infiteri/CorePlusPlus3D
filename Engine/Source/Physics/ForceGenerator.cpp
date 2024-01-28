#include "ForceGenerator.h"
#include "Core/Engine.h"

namespace Core
{
    ForceRegistry::ForceRegistry()
    {
    }

    ForceRegistry::~ForceRegistry()
    {
    }

    void ForceRegistry::AddGenerator(ForceGenerator *generator)
    {
        generators.push_back(generator);
    }

    void ForceRegistry::UpdateBodyWithAllGenerators(PhysicsBody *body)
    {
        int i = 0;
        while (i < generators.size())
        {
            i++;
            generators[i]->Implement(body, Engine::GetDeltaTime());
        }
    }
}