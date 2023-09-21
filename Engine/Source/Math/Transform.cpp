#include "Transform.h"

namespace Core
{
    Transform::Transform()
    {
    }

    Transform::~Transform()
    {
    }

    Matrix4 Transform::GetMatrix()
    {
        Matrix4 translation = Matrix4::Translate(&position);
        Matrix4 rotated = Matrix4::RotationXYZ(&rotation);
        Matrix4 scaled = Matrix4::Scale(&scale);

        return Matrix4::Multiply(Matrix4::Multiply(translation, rotated), scaled);
    }
}