#include "Camera.h"

namespace Core
{
    Camera::Camera()
    {
    }

    Camera::~Camera()
    {
    }

    Matrix4 Camera::GetViewMatrix()
    {
        Matrix4 translation = Matrix4::Translate(transform.GetPosition());
        Matrix4 rotationMatrix = Matrix4::RotationZYX(transform.GetRotation());
        Matrix4 mix = Matrix4::Multiply(&translation, &rotationMatrix);
        return Matrix4::Invert(&mix);
    }
}