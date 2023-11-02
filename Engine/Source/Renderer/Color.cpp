#include "Color.h"
#include "Core/Logger.h"

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

    void Color::Set(Color *other)
    {
        Set(other->r, other->g, other->b, other->a);
    }

    void Color::Set4(float colors[4], float multiplier)
    {
        Set(colors[0] * multiplier,
            colors[1] * multiplier,
            colors[2] * multiplier,
            colors[3] * multiplier);
    }

    void Color::Set3(float colors[3], float multiplier)
    {
        Set(colors[0] * multiplier,
            colors[1] * multiplier,
            colors[2] * multiplier,
            a);
    }
}