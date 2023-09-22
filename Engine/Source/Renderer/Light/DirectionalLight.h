#pragma once

#include "Light.h"

namespace Core
{
    class CE_API DirectionalLight : public Light
    {
    private:
        Vector3 direction;

    public:
        DirectionalLight();
        ~DirectionalLight();

        void Update();

        inline Vector3 *GetDirection() { return &direction; };
    };
}