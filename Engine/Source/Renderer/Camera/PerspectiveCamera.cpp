#include "PerspectiveCamera.h"

#include "Core/Engine.h"

#include "Math/Math.h"

namespace Core
{
    PerspectiveCamera::PerspectiveCamera()
    {
        this->FOV = Math::DegToRad(90);
        this->near = 0.01;
        this->far = 1000;

        UpdateProjection((float)Engine::GetWindowInstance()->GetWidth() / (float)Engine::GetWindowInstance()->GetHeight());
    }

    PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float near, float far)
    {
        this->FOV = fov;
        this->near = near;
        this->far = far;

        UpdateProjection(aspect);
    }

    PerspectiveCamera::~PerspectiveCamera()
    {
    }

    void PerspectiveCamera::UpdateProjection(float aspect)
    {
        projection = Matrix4::Perspective(FOV, aspect, near, far);
    }

    float PerspectiveCamera::GetFOV()
    {
        return FOV;
    }

    void PerspectiveCamera::SetFOV(float val)
    {
        FOV = val;
    }

    float PerspectiveCamera::GetNear()
    {
        return near;
    }

    void PerspectiveCamera::SetNear(float val)
    {
        near = val;
    }

    float PerspectiveCamera::GetFar()
    {
        return far;
    }

    void PerspectiveCamera::SetFar(float val)
    {
        far = val;
    }
}