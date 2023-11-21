#include "Transform.h"

#include <cmath>

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

    void Transform::From(Transform *other)

    {
        position.Set(&other->position);
        rotation.Set(&other->rotation);
        scale.Set(&other->scale);
    }

    // TODO: FIX
    void Transform::FromMatrix(Matrix4 *matrix, bool invert)
    {
        position = Vector3(matrix->data[12], matrix->data[13], matrix->data[14]);

        if (invert)
            *matrix = Matrix4::Invert(*matrix);

        // Extract rotation angles
        rotation.x = std::asin(-matrix->data[9]);
        rotation.y = std::atan2(matrix->data[8], matrix->data[10]);
        rotation.z = std::atan2(matrix->data[1], matrix->data[5]);

        if (invert)
            *matrix = Matrix4::Invert(*matrix);

        // Extract scale
        scale.x = sqrt(matrix->data[0] * matrix->data[0] + matrix->data[1] * matrix->data[1] + matrix->data[2] * matrix->data[2]);
        scale.y = sqrt(matrix->data[4] * matrix->data[4] + matrix->data[5] * matrix->data[5] + matrix->data[6] * matrix->data[6]);
        scale.z = sqrt(matrix->data[8] * matrix->data[8] + matrix->data[9] * matrix->data[9] + matrix->data[10] * matrix->data[10]);
    }

    Matrix4 Transform::GetMatrix()
    {
        Matrix4 translation = Matrix4::Translate(&position);
        Matrix4 rotated = Matrix4::RotationZYX(&rotation);
        Matrix4 scaled = Matrix4::Scale(&scale);

        return Matrix4::Multiply(Matrix4::Multiply(translation, rotated), scaled);
    }
}