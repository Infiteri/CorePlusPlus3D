#pragma once

#include "Base.h"

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

        CE_API float Sin(float x);
        CE_API float Cos(float x);
        CE_API float Tan(float x);
    }
}