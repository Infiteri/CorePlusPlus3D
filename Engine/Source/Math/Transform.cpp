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
        if (invert)
        {
            *matrix = Matrix4::Invert(*matrix);

            position = Vector3(matrix->data[12], matrix->data[13], matrix->data[14]);

            // Extract rotation matrix
            Matrix4 rotationMatrix = *matrix;
            rotationMatrix.data[12] = rotationMatrix.data[13] = rotationMatrix.data[14] = 0;
            rotationMatrix.data[15] = 1;

            // Extract scaling matrix
            Matrix4 scaleMatrix = *matrix;
            scaleMatrix.data[0] /= scale.x;
            scaleMatrix.data[1] /= scale.x;
            scaleMatrix.data[2] /= scale.x;
            scaleMatrix.data[4] /= scale.y;
            scaleMatrix.data[5] /= scale.y;
            scaleMatrix.data[6] /= scale.y;
            scaleMatrix.data[8] /= scale.z;
            scaleMatrix.data[9] /= scale.z;
            scaleMatrix.data[10] /= scale.z;

            // Extract rotation angles (assuming RotationXYZ uses degrees)
            rotation.x = std::asin(-rotationMatrix.data[9]);
            rotation.y = std::atan2(rotationMatrix.data[8], rotationMatrix.data[10]);
            rotation.z = std::atan2(rotationMatrix.data[1], rotationMatrix.data[5]);

            *matrix = Matrix4::Invert(*matrix);

            scale.x = scaleMatrix.data[0];
            scale.y = scaleMatrix.data[5];
            scale.z = scaleMatrix.data[10];
        }
        else
        {
            position = Vector3(matrix->data[12], matrix->data[13], matrix->data[14]);

            // Extract rotation matrix
            Matrix4 rotationMatrix = *matrix;
            rotationMatrix.data[12] = rotationMatrix.data[13] = rotationMatrix.data[14] = 0;
            rotationMatrix.data[15] = 1;

            // Extract scaling matrix
            Matrix4 scaleMatrix = *matrix;
            scaleMatrix.data[0] /= scale.x;
            scaleMatrix.data[1] /= scale.x;
            scaleMatrix.data[2] /= scale.x;
            scaleMatrix.data[4] /= scale.y;
            scaleMatrix.data[5] /= scale.y;
            scaleMatrix.data[6] /= scale.y;
            scaleMatrix.data[8] /= scale.z;
            scaleMatrix.data[9] /= scale.z;
            scaleMatrix.data[10] /= scale.z;

            // Extract rotation angles (assuming RotationXYZ uses degrees)
            rotation.x = std::asin(-rotationMatrix.data[9]);
            rotation.y = std::atan2(rotationMatrix.data[8], rotationMatrix.data[10]);
            rotation.z = std::atan2(rotationMatrix.data[1], rotationMatrix.data[5]);

            scale.x = scaleMatrix.data[0];
            scale.y = scaleMatrix.data[5];
            scale.z = scaleMatrix.data[10];
        }
    }

    Matrix4 Transform::GetMatrix()
    {
        Matrix4 translation = Matrix4::Translate(&position);
        Matrix4 rotated = Matrix4::RotationZYX(&rotation);
        Matrix4 scaled = Matrix4::Scale(&scale);

        return Matrix4::Multiply(Matrix4::Multiply(translation, rotated), scaled);
    }
}