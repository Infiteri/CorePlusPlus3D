#pragma once

#include "Base.h"

#include "Renderer/Color.h"
#include "Math/Transform.h"
#include "Math/Matrix4.h"

namespace Core
{
    /// @brief A light isn't truly a 3D object, it has proprieties like color, position and strength. Also a init, Destroy and Update functions aren't necessary, extending of off Object3D would waste memory.
    class Light
    {
    protected:
        Color color;
        Transform transform;

    public:
        Light();
        ~Light();

        virtual void Update();

        inline Matrix4 GetTransformMatrix() { return transform.GetMatrix(); };
        inline Vector3 *GetPosition() { return transform.GetPosition(); };
        inline Vector3 *GetRotation() { return transform.GetRotation(); };
        inline Vector3 *GetScale() { return transform.GetScale(); };
        inline Transform *GetTransform() { return &transform; };
    };
}