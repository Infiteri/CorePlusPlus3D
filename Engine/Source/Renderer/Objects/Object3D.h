#pragma once

#include "Base.h"
#include "Math/Vectors.h"
#include "Math/Matrix4.h"
#include "Math/Transform.h"

namespace Core
{
    /// @brief Core object for most 3D objects (Camera, Meshes, etc...);
    class CE_API Object3D
    {
    protected:
        Transform transform;
        bool init = false;

    public:
        Object3D();
        ~Object3D();

        virtual void Destroy();

        virtual void Init();
        virtual void Render();
        virtual void Update();

        inline Matrix4 GetTransformMatrix() { return transform.GetMatrix(); };
        inline Vector3 *GetPosition() { return transform.GetPosition(); };
        inline Vector3 *GetRotation() { return transform.GetRotation(); };
        inline bool GetInit() { return init; };
    };
}