#pragma once

#include "Geometry.h"

namespace Core
{
    class CE_API BoxGeometry : public Geometry
    {
    public:
        float width;
        float height;
        float depth;

        BoxGeometry(float _width, float _height, float _depth);
        ~BoxGeometry();
    };

}