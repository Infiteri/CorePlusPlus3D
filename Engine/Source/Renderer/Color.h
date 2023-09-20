#pragma once

#include "Base.h"

namespace Core
{
    class CE_API Color
    {
    public:
        float r;
        float g;
        float b;
        float a;

        Color();
        Color(float r, float g, float b, float a);
        ~Color();

        void Set(float _r, float _g, float _b, float _a);
    };
}
