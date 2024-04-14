#include "Vectors.h"
#include <cmath>
#include <algorithm>

namespace Core
{
    static float dot(const Vector3 &v1, const Vector3 &v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    Vector2::Vector2()
    {
        Set(0, 0);
    }

    Vector2::Vector2(float x, float y)
    {
        Set(x, y);
    }

    Vector2::~Vector2()
    {
    }

    void Vector2::Set(float x, float y)
    {
        this->x = x;
        this->y = y;
    }

    void Vector2::Set(Vector2 *v)
    {
        Set(v->x, v->y);
    }

    bool Vector2::NotZero() const
    {
        return (x != 0.0f || y != 0.0f);
    }

    Vector3::Vector3()
    {
        Set(0, 0, 0);
    }

    Vector3::Vector3(float x, float y, float z)
    {
        Set(x, y, z);
    }

    Vector3::Vector3(const Vector3 &other)
    {
        Set(other);
    }

    Vector3::~Vector3()
    {
    }

    void Vector3::Set(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    void Vector3::Normalize()
    {
        float magnitude = std::sqrt(x * x + y * y + z * z);
        x /= magnitude;
        y /= magnitude;
        z /= magnitude;
    }

    void Vector3::Set(Vector3 *v)
    {
        Set(v->x, v->y, v->z);
    }

    void Vector3::Set(const Vector3 &v)
    {
        Set(v.x, v.y, v.z);
    }

    Vector3 Vector3::Reflect(const Vector3 &other)
    {
        float dotProduct = dot(*this, other);
        Vector3 ref = *this - 2.0f;
        Vector3 reflection = ref * other * dotProduct;
        return reflection;
    }

    Vector3 Vector3::Max(const Vector3 &other)
    {
        return Vector3(
            std::max(x, other.x),
            std::max(y, other.y),
            std::max(z, other.z));
    }

    float Vector3::Max()
    {
        return std::max({x, y, z});
    }

    bool Vector3::NotZero() const
    {
        return (x != 0.0f || y != 0.0f || z != 0.0f);
    }

    Vector4::Vector4()
    {
        Set(0, 0, 0, 0);
    }

    Vector4::Vector4(float x, float y, float z, float w)
    {
        Set(x, y, z, w);
    }

    Vector4::~Vector4()
    {
    }

    void Vector4::Set(float x, float y, float z, float w)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    void Vector4::Set(Vector4 *v)
    {
        Set(v->x, v->y, v->z, v->w);
    }

    bool Vector4::NotZero() const
    {
        return (x != 0.0f || y != 0.0f || z != 0.0f || w != 0.0f);
    }
}