#pragma once

#include "Base.h"
#include "Math/Vectors.h"
#include "Math/Matrix4.h"

namespace Core
{
    /// @brief Uses a 0.5 point.
    struct CE_API AABB
    {
        /// @brief S Y Z
        Vector3 Position;

        /// @brief WIDTH HEIGHT DEPTH
        Vector3 Sizes;

        /// @brief Direction
        Vector3 Direction;

        Matrix4 *TransformMatrix;

        bool Collides(AABB *other);
        bool CollidesX(const AABB *other);
        bool CollidesY(const AABB *other);
        bool CollidesZ(const AABB *other);
        Vector3 GetAxis(int a) const;
    };
}