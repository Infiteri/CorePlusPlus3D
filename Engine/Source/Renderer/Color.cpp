#include "Color.h"

namespace Core
{
    Color::Color()
    {
        Set(255, 255, 255, 255);
    }

    Color::Color(float r, float g, float b, float a)
    {
        Set(r, g, b, a);
    }

    Color::~Color()
    {
    }

    void Color::Set(float _r, float _g, float _b, float _a)
    {
        r = _r;
        g = _g;
        b = _b;
        a = _a;
    }
}