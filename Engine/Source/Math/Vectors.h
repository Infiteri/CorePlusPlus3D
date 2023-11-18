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
        void Set(Vector3 *v);

        void operator+=(const Vector3 &o)
        {
            x += o.x;
            y += o.y;
            z += o.z;
        };

        Vector3 operator*(const Vector3 &o) const
        {
            return {x * o.x, y * o.y, z * o.z};
        }
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
    };
}