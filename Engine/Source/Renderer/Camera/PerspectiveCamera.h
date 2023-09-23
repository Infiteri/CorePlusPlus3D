#pragma once

#include "Camera.h"

namespace Core
{
    class CE_API PerspectiveCamera : public Camera
    {
    private:
        float FOV;
        float near;
        float far;
        float aspect;

    public:
        PerspectiveCamera();
        PerspectiveCamera(float fov, float aspect, float near, float far);
        ~PerspectiveCamera();

        void UpdateProjection(float aspect);

        /// @brief Will use last aspect
        void UpdateProjection();

        float GetFOV();
        void SetFOV(float val);

        float GetNear();
        void SetNear(float val);

        float GetFar();
        void SetFar(float val);
    };
}