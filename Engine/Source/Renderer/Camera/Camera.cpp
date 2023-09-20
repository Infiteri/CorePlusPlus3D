#include "Camera.h"

namespace Core
{
    Camera::Camera()
    {
    }

    Camera::~Camera()
    {
    }

    void Camera::UpdateProjection()
    {
    }

    Matrix4 Camera::GetViewMatrix()
    {
        Matrix4 translation = Matrix4::Translate(&position);
        Matrix4 rotationMatrix = Matrix4::RotationZYX(&rotation);
        Matrix4 mix = Matrix4::Multiply(&translation, &rotationMatrix);
        return Matrix4::Invert(&mix);
    }
}