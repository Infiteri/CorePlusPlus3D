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
        CE_PROFILE_FUNCTION();

        Matrix4 translation = Matrix4::Translate(&transform.position);
        Matrix4 rotationMatrix = Matrix4::RotationZYX(&transform.rotation);
        Matrix4 mix = Matrix4::Multiply(&translation, &rotationMatrix);

        return Matrix4::Invert(&mix);
    }
}