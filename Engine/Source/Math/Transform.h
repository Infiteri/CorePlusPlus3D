#pragma once

#include "Base.h"
#include "Vectors.h"
#include "Matrix4.h"

namespace Core
{
    class CE_API Transform
    {
    private:
        Vector3 position;
        Vector3 rotation;
        Vector3 scale;

    public:
        Transform();
        ~Transform();

        inline Vector3 *GetPosition() { return &position; };
        inline Vector3 GetRawPosition() { return position; };

        inline Vector3 *GetRotation() { return &rotation; };
        inline Vector3 GetRawRotation() { return rotation; };

        inline Vector3 *GetScale() { return &scale; };
        inline Vector3 GetRawScale() { return scale; };

        Matrix4 GetMatrix();
    };
}
