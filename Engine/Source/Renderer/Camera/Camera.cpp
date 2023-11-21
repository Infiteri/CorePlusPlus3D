#include "Camera.h"
#include "Core/Logger.h"

namespace Core
{
    Camera::Camera()
    {
    }

    Camera::~Camera()
    {
    }

    void Camera::SetViewMatrix(const Matrix4 &other)
    {
        if (matrixMode != InputMatrix)
        {
            CE_CORE_WARN("Camera::SetViewMatrix: Invalid mode.");
            return;
        }

        view.From(other);
    }

    Matrix4 Camera::GetViewMatrix()
    {
        CE_PROFILE_FUNCTION();

        if (matrixMode == TransformMatrix)
        {
            Matrix4 translation = Matrix4::Translate(&transform.position);
            Matrix4 rotationMatrix = Matrix4::RotationZYX(&transform.rotation);
            view = Matrix4::Multiply(&translation, &rotationMatrix);
        }

        return Matrix4::Invert(&view);
    }

    void Camera::SetMatrixMode(MatrixMode mode)
    {
        matrixMode = mode;
    }
}