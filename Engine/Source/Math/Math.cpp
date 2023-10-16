#include "Math.h"

#include <cmath>

namespace Core
{
    namespace Math
    {
        int Max(int a, int b)
        {
            if (a > b)
                return a;
            else
                return b;
        }

        int Min(int a, int b)
        {
            if (a < b)
                return a;
            else
                return b;
        }

        float DegToRad(float a)
        {
            return a * CE_DEG_TO_RAD;
        }

        float RadToDeg(float a)
        {
            return a * CE_RAD_TO_DEG;
        }

        void DecomposePosition(float *data, Vector3 *out)
        {
            out->x = data[12];
            out->y = data[13];
            out->z = data[14];
        }

        void DecomposeRotation(float *data, Vector3 *out)
        {
            float m00 = data[0];
            float m01 = data[1];
            float m02 = data[2];
            float m10 = data[4];
            float m11 = data[5];
            float m12 = data[6];
            float m20 = data[8];
            float m21 = data[9];
            float m22 = data[10];

            // Calculate the scaling factors
            float scaleX = Sqrt(m00 * m00 + m01 * m01 + m02 * m02);
            float scaleY = Sqrt(m10 * m10 + m11 * m11 + m12 * m12);
            float scaleZ = Sqrt(m20 * m20 + m21 * m21 + m22 * m22);

            // Remove scaling from the matrix to extract the rotation part
            m00 /= scaleX;
            m01 /= scaleX;
            m02 /= scaleX;

            m10 /= scaleY;
            m11 /= scaleY;
            m12 /= scaleY;

            m20 /= scaleZ;
            m21 /= scaleZ;
            m22 /= scaleZ;

            // Calculate rotation angles using the extracted rotation matrix
            float angleX = atan2f(m12, m22);
            float angleY = atan2f(-m02, sqrtf(m00 * m00 + m01 * m01));
            float angleZ = atan2f(m01, m00);

            // Convert angles from radians to degrees if desired
            out->x = angleX;
            out->y = angleY;
            out->z = angleZ;
        }

        void DecomposeScale(float *data, Vector3 *out)
        {
            // Calculate the scaling factors
            float scaleX = sqrt(data[0] * data[0] + data[1] * data[1] + data[2] * data[2]);
            float scaleY = sqrt(data[4] * data[4] + data[5] * data[5] + data[6] * data[6]);
            float scaleZ = sqrt(data[8] * data[8] + data[9] * data[9] + data[10] * data[10]);

            out->x = scaleX;
            out->y = scaleY;
            out->z = scaleZ;
        }

        float Sqrt(float x)

        {
            return sqrtf(x);
        }

        float Sin(float x)
        {
            return sinf(x);
        }

        float Cos(float x)
        {
            return cosf(x);
        }

        float Tan(float x)
        {
            return tanf(x);
        }
    }
}