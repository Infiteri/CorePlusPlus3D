#pragma once

#include "Base.h"
#include "AABB.h"

namespace Core
{
    class RigidBody;
    class CE_API Collider
    {
    public:
        enum ColliderType
        {
            TypeAABB
        };

    protected:
        ColliderType Type;

    public:
        Collider();
        ~Collider();

        bool Intersects(Collider *Other);
        inline ColliderType GetTYpe() { return Type; };

        template <typename T>
        T *As() { return (T *)this; };

        virtual void UpdateFromBody(RigidBody *b){};
    };

    class AABBCollider : public Collider
    {
    private:
        AABB aabb;

    public:
        AABBCollider();
        ~AABBCollider();

        inline AABB *GetAABB() { return &aabb; };
        void UpdateFromBody(RigidBody *b);
    };

}