#pragma once

#include "PerspectiveCamera.h"

namespace Core
{
    class CE_API PerspectiveMovement
    {
    public:
        float speed = 0.02f;
        float rotation = 0.01f;
        float sensitivity = 0.001f;

        PerspectiveMovement();
        ~PerspectiveMovement();

        void UpdateRotation(PerspectiveCamera *camera);
        void UpdatePosition(PerspectiveCamera *camera);
        void UpdateWithMouse(PerspectiveCamera *camera);
    };

}