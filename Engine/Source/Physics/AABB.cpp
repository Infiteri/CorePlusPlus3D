#include "AABB.h"

namespace Core
{
    bool AABB::Collides(const AABB *other)
    {
        bool coll = (CollidesX(other) && CollidesY(other) && CollidesZ(other));

        if (coll)
        {
            Vector3 dist1 = other->Position - Sizes;
            Vector3 dist2 = Position - other->Sizes;
            Vector3 dist = dist1.Max(dist2);

            float distance = dist.Max();

            Direction = Vector3(distance, distance, distance);
        }

        return coll;
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
}