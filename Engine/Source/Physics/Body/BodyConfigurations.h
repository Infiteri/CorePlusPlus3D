#pragma once
#include "Base.h"
#include "Math/Vectors.h"

namespace Core
{
    struct RigidBodyConfiguration
    {
        Vector3 Velocity;
        Vector3 Acceleration;
        Vector3 Size;
        class Actor *Owner;
        float Mass;
        float Damping;
    };
}
