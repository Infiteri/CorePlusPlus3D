#pragma once

#include "Base.h"
#include "Math/Vectors.h"
#include "Math/Matrix4.h"

namespace Core
{
    /// @brief Core object for most 3D objects (Camera, Meshes, etc...);
    class CE_API Object3D
    {
    protected:
        Vector3 position;
        Vector3 rotation;
        bool init = false;

    public:
        Object3D();
        ~Object3D();

        virtual void Destroy();

        virtual void Init();
        virtual void Render();
        virtual void Update();

        inline Matrix4 GetTransformMatrix() { return Matrix4::Multiply(Matrix4::Translate(&position), Matrix4::RotationZYX(position)); };
        inline Vector3 *GetPosition() { return &position; };
        inline Vector3 *GetRotation() { return &rotation; };
        inline bool GetInit() { return init; };
    };
}