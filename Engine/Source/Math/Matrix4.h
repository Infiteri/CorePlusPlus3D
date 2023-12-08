#pragma once

#include "Base.h"
#include "Vectors.h"

namespace Core
{
    class CE_API Matrix4
    {
    public:
        Matrix4();
        ~Matrix4();

        float data[16];
        void From(Matrix4 *other);
        void From(Matrix4 other);
        void From(float *data);
        void Normalized();

        Vector3 TransformVector(const Vector3 &vector) const
        {
            float x = data[0] * vector.x + data[1] * vector.y + data[2] * vector.z + data[3];
            float y = data[4] * vector.x + data[5] * vector.y + data[6] * vector.z + data[7];
            float z = data[8] * vector.x + data[9] * vector.y + data[10] * vector.z + data[11];

            return Vector3(x, y, z);
        }

        static Matrix4 Identity();
        static Matrix4 Empty();

        // WIP: Projections and transformations

        static Matrix4 Perspective(float fov, float aspect, float near, float far);
        static Matrix4 Translate(float x, float y, float z);
        static Matrix4 Translate(Vector3 *vec);

        static Matrix4 Multiply(Matrix4 *a, Matrix4 *b);
        static Matrix4 Invert(Matrix4 *a);

        static Matrix4 Multiply(Matrix4 a, Matrix4 b);
        static Matrix4 Invert(Matrix4 a);

        static Matrix4 RotationXYZ(Vector3 vec);
        static Matrix4 RotationXYZ(Vector3 *vec);
        static Matrix4 RotationXYZ(float rotationX, float rotationY, float rotationZ);

        static Matrix4 RotationZYX(Vector3 vec);
        static Matrix4 RotationZYX(Vector3 *vec);
        static Matrix4 RotationZYX(float rotationX, float rotationY, float rotationZ);

        static Matrix4 RotationX(float rotation);
        static Matrix4 RotationY(float rotation);
        static Matrix4 RotationZ(float rotation);

        static Vector3 Forward(Matrix4 mat);
        static Vector3 Forward(Matrix4 *mat);

        static Vector3 Right(Matrix4 mat);
        static Vector3 Right(Matrix4 *mat);

        static Matrix4 Scale(Vector3 *vec);
        static Matrix4 Scale(float x, float y, float z);
    };
}