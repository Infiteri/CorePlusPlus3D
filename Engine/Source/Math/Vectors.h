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
        }

        Vector2 operator+(const Vector2 &o) const
        {
            return {x + o.x, y + o.y};
        }

        Vector2 operator-(const Vector2 &o) const
        {
            return {x - o.x, y - o.y};
        }

        Vector2 operator*(float scalar) const
        {
            return {x * scalar, y * scalar};
        }

        Vector2 operator/(float scalar) const
        {
            return {x / scalar, y / scalar};
        }

        bool operator==(const Vector2 &o) const
        {
            return x == o.x && y == o.y;
        }

        bool operator!=(const Vector2 &o) const
        {
            return !(*this == o);
        }

        bool NotZero() const;
    };

    class CE_API Vector3
    {
    public:
        float x;
        float y;
        float z;

        Vector3();
        Vector3(float x, float y, float z);
        Vector3(const Vector3 &other);
        ~Vector3();

        void Set(float x, float y, float z);
        void Normalize();
        void Set(Vector3 *v);
        void Set(const Vector3 &v);
        float Dot(const Vector3 &a) const;

        Vector3 Reflect(const Vector3 &other);

        void operator+=(const Vector3 &o)
        {
            x += o.x;
            y += o.y;
            z += o.z;
        }

        void operator-=(const Vector3 &o)
        {
            x -= o.x;
            y -= o.y;
            z -= o.z;
        }

        Vector3 operator+(const Vector3 &o) const
        {
            return {x + o.x, y + o.y, z + o.z};
        }

        Vector3 operator-(const Vector3 &o) const
        {
            return {x - o.x, y - o.y, z - o.z};
        }

        Vector3 operator-(Vector3 *o) const
        {
            return {x - o->x, y - o->y, z - o->z};
        }

        Vector3 operator-() const
        {
            return {-x, -y, -z};
        }

        Vector3 operator*(const Vector3 &o)
        {
            return {x * o.x, y * o.y, z * o.z};
        };

        Vector3 operator*(float scalar) const
        {
            return {x * scalar, y * scalar, z * scalar};
        }

        Vector3 operator/(float scalar) const
        {
            return {x / scalar, y / scalar, z / scalar};
        }

        Vector3 operator-(float scalar) const
        {
            return {x - scalar, y - scalar, z - scalar};
        }

        void operator*=(float scalar)
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;
        }

        bool operator==(const Vector3 &o) const
        {
            return x == o.x && y == o.y && z == o.z;
        }

        bool operator!=(const Vector3 &o) const
        {
            return !(*this == o);
        }

        Vector3 Max(const Vector3 &other);
        float Max();

        bool NotZero() const;
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
        }

        Vector4 operator+(const Vector4 &o) const
        {
            return {x + o.x, y + o.y, z + o.z, w + o.w};
        }

        Vector4 operator-(const Vector4 &o) const
        {
            return {x - o.x, y - o.y, z - o.z, w - o.w};
        }

        Vector4 operator*(float scalar) const
        {
            return {x * scalar, y * scalar, z * scalar, w * scalar};
        }

        Vector4 operator/(float scalar) const
        {
            return {x / scalar, y / scalar, z / scalar, w / scalar};
        }

        bool operator==(const Vector4 &o) const
        {
            return x == o.x && y == o.y && z == o.z && w == o.w;
        }

        bool operator!=(const Vector4 &o) const
        {
            return !(*this == o);
        }

        bool NotZero() const;
    };
}
