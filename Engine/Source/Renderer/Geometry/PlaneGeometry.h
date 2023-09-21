#pragma once

#include "Geometry.h"

namespace Core
{
    class CE_API PlaneGeometry : public Geometry
    {
    public:
        float width;
        float height;

        PlaneGeometry(float _width, float _height);
        ~PlaneGeometry();
    };

}