#include "Transform.h"

namespace Core
{
    Transform::Transform()
    {
        position = Vector3(0, 0, 0);
        rotation = Vector3(0, 0, 0);
        scale = Vector3(1, 1, 1);
    }

    Transform::~Transform()
    {
    }
    
    void Transform::From(Transform* other)
    {
        position.Set(other->GetPosition());
        rotation.Set(other->GetRotation());
        scale.Set(other->GetScale());
    }

    Matrix4 Transform::GetMatrix()
    {
        Matrix4 translation = Matrix4::Translate(&position);
        Matrix4 rotated = Matrix4::RotationZYX(&rotation);
        Matrix4 scaled = Matrix4::Scale(&scale);

        return Matrix4::Multiply(Matrix4::Multiply(translation, rotated), scaled);
    }
}