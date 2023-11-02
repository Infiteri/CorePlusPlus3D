#pragma once

#include "Base.h"

#include "Vectors.h"

#define CE_PI 3.14159265358979323846
#define CE_2PI CE_PI * 2
#define CE_HALF_PI CE_PI / 2

#define CE_RAD_TO_DEG 180 / CE_PI
#define CE_DEG_TO_RAD CE_PI / 180

namespace Core
{
    namespace Math
    {
        CE_API int Max(int a, int b);
        CE_API int Min(int a, int b);
        CE_API float DegToRad(float a);
        CE_API float RadToDeg(float a);

        CE_API void DecomposePosition(float *data, Vector3 *out);
        CE_API void DecomposeRotation(float *data, Vector3 *out);
        CE_API void DecomposeScale(float *data, Vector3 *out);
        CE_API void DecomposeTransform(float *data, Vector3 *t, Vector3 *r, Vector3 *s);

        CE_API float Sqrt(float x);
        CE_API float Sin(float x);
        CE_API float Cos(float x);
        CE_API float Tan(float x);

        CE_API float RandomFloat();
        CE_API float RandomFloat(float min, float max);
        CE_API float Lerp(float min, float max, float t);
        CE_API float Clamp(float min, float max, float t);
    }
}