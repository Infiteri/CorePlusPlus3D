#pragma once

#include "Base.h"
#include "Vectors.h"
#include "Matrix4.h"

namespace Core
{
    class CE_API Transform
    {
    public:
        Vector3 position;
        Vector3 rotation;
        Vector3 scale;

        Transform();
        ~Transform();

        void From(Transform *other);
        Matrix4 GetMatrix();
    };
}
