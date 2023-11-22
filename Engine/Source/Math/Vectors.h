#pragma once

#include "Base.h"

namespace Core
{
    class CE_API Vector2
    {
    public:
        float x;
        float y;

        Vector2();
        Vector2(float x, float y);
        ~Vector2();

        void Set(float x, float y);
        void Set(Vector2 *v);

        void operator+=(const Vector2 &o)
        {
            x += o.x;
            y += o.y;
        };

        Vector2 operator*(const Vector2 &o) const
        {
            return {x * o.x, y * o.y};
        }

        bool NotZero();
    };

    class CE_API Vector3
    {
    public:
        float x;
        float y;
        float z;

        Vector3();
        Vector3(float x, float y, float z);
        ~Vector3();

        void Set(float x, float y, float z);
        void Normalize();
        void Set(Vector3 *v);

        void operator+=(const Vector3 &o)
        {
            x += o.x;
            y += o.y;
            z += o.z;
        };

        Vector3 operator+(const Vector3 &o)
        {
            return {x + o.x, y + o.y, z + o.z};
        };

        Vector3 operator*(const Vector3 &o) const
        {
            return {x * o.x, y * o.y, z * o.z};
        }

        Vector3 operator*(float f) const
        {
            return {x * f, y * f, z * f};
        }

        bool NotZero();
    };

    class CE_API Vector4
    {
    public:
        float x;
        float y;
        float z;
        float w;

        Vector4();
        Vector4(float x, float y, float z, float w);
        ~Vector4();

        void Set(float x, float y, float z, float w);
        void Set(Vector4 *v);

        void operator+=(const Vector4 &o)
        {
            x += o.x;
            y += o.y;
            z += o.z;
            w += o.w;
        };

        Vector4 operator*(const Vector4 &o) const
        {
            return {x * o.x, y * o.y, z * o.z, w * o.w};
        }

        bool NotZero();
    };
}