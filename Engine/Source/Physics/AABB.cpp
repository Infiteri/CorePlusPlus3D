#include "AABB.h"
#include "Math/Math.h"

namespace Core
{
    float TransformToAxis(AABB *aabb, const Vector3 &axis)
    {
        return aabb->Sizes.x * Math::Abs(axis.Dot(aabb->GetAxis(0))) +
               aabb->Sizes.y * Math::Abs(axis.Dot(aabb->GetAxis(1))) +
               aabb->Sizes.z * Math::Abs(axis.Dot(aabb->GetAxis(2)));
    }

    bool CollidesOnAxis(AABB *one, AABB *two, const Vector3 &axis)
    {
        // Use the rotation part of the matrix to transform the axis
        float oneProject = TransformToAxis(one, axis);
        float twoProject = TransformToAxis(two, axis);
        Vector3 toCenter = two->GetAxis(3) - one->GetAxis(3);
        float distance = Math::Abs(toCenter.Dot(axis));
        return (distance < oneProject + twoProject);
    }

    bool AABB::Collides(AABB *other)
    {
        if (!other)
        {
            return false;
        }

        for (int i = 0; i < 3; ++i)
        {
            Vector3 axis = GetAxis(i); // Normalize the axis
            axis.Normalize();

            if (!CollidesOnAxis(this, other, axis))
            {
                return false; // No overlap on this axis, early exit
            }
        }

        return true; // Overlap on all axes
    }

    bool AABB::CollidesX(const AABB *other)
    {
        if (!other)
        {
            return false;
        }

        float left1 = Position.x - Sizes.x;
        float right1 = Position.x + Sizes.x;
        float left2 = other->Position.x - other->Sizes.x;
        float right2 = other->Position.x + other->Sizes.x;
        bool horizontalOverlap = (left1 <= right2) && (right1 >= left2);
        return horizontalOverlap;
    }

    bool AABB::CollidesY(const AABB *other)
    {
        float left1 = Position.y - Sizes.y;
        float right1 = Position.y + Sizes.y;
        float left2 = other->Position.y - other->Sizes.y;
        float right2 = other->Position.y + other->Sizes.y;
        bool horizontalOverlap = (left1 <= right2) && (right1 >= left2);
        return horizontalOverlap;
    }

    bool AABB::CollidesZ(const AABB *other)
    {
        float left1 = Position.z - Sizes.z;
        float right1 = Position.z + Sizes.z;
        float left2 = other->Position.z - other->Sizes.z;
        float right2 = other->Position.z + other->Sizes.z;
        bool horizontalOverlap = (left1 <= right2) && (right1 >= left2);
        return horizontalOverlap;
    }

    Vector3 AABB::GetAxis(int a) const
    {
        switch (a)
        {
        case 0:
            return Vector3(1, 0, 0);
            break;

        case 1:
            return Vector3(0, 1, 0);
            break;

        case 2:
            return Vector3(0, 0, 1);
            break;

        case 3:
            return TransformMatrix->TransformVector(Position + (Sizes * 0.5f /** TODO: Should be in half or not*/));
            break;

        default:
            return Vector3(0, 0, 0);
            break;
        }
    }
}