#include "PerspectiveMovement.h"

#include "Core/Input.h"
#include "Core/Engine.h"
#include "Core/Logger.h"

#include "Math/Math.h"

namespace Core
{
    PerspectiveMovement::PerspectiveMovement()
    {
        speed = 5.0f;
        rotation = 5.0f;
        sensitivity = 0.01f;
    }

    PerspectiveMovement::~PerspectiveMovement()
    {
    }

    void PerspectiveMovement::UpdateRotation(PerspectiveCamera *camera)
    {
        float dt = Engine::GetDeltaTime();

        if (Input::GetKey(Keys::W))
        {
            Matrix4 cameraMatrixRotation = Matrix4::RotationXYZ(camera->GetRotation());
            Vector3 way = Matrix4::Forward(&cameraMatrixRotation);
            camera->GetPosition()->x += way.x * speed * dt;
            camera->GetPosition()->y += way.y * speed * dt;
            camera->GetPosition()->z += way.z * speed * dt;
        }

        if (Input::GetKey(Keys::S))
        {
            Matrix4 cameraMatrixRotation = Matrix4::RotationXYZ(camera->GetRotation());
            Vector3 way = Matrix4::Forward(&cameraMatrixRotation);
            camera->GetPosition()->x -= way.x * speed * dt;
            camera->GetPosition()->y -= way.y * speed * dt;
            camera->GetPosition()->z -= way.z * speed * dt;
        }

        if (Input::GetKey(Keys::A))
        {
            Matrix4 cameraMatrixRotation = Matrix4::RotationXYZ(camera->GetRotation());
            Vector3 way = Matrix4::Right(&cameraMatrixRotation);
            camera->GetPosition()->x -= way.x * speed * dt;
            camera->GetPosition()->y -= way.y * speed * dt;
            camera->GetPosition()->z -= way.z * speed * dt;
        }

        if (Input::GetKey(Keys::D))
        {
            Matrix4 cameraMatrixRotation = Matrix4::RotationXYZ(camera->GetRotation());
            Vector3 way = Matrix4::Right(&cameraMatrixRotation);
            camera->GetPosition()->x += way.x * speed * dt;
            camera->GetPosition()->y += way.y * speed * dt;
            camera->GetPosition()->z += way.z * speed * dt;
        }
    }

    void PerspectiveMovement::UpdatePosition(PerspectiveCamera *camera)
    {
        float dt = Engine::GetDeltaTime();

        if (Input::GetKey(Keys::Left))
        {
            camera->GetRotation()->y -= rotation * dt;
        }

        if (Input::GetKey(Keys::Right))
        {
            camera->GetRotation()->y += rotation * dt;
        }

        if (Input::GetKey(Keys::Up))
        {
            camera->GetRotation()->x += rotation * dt;
        }

        if (Input::GetKey(Keys::Down))
        {
            camera->GetRotation()->x -= rotation * dt;
        }
    }

    void PerspectiveMovement::UpdateWithMouse(PerspectiveCamera *camera)
    {
        // Check if the right mouse button is pressed
        if (Input::GetButton(Buttons::Right))
        {
            // Lock the mouse mode to handle continuous input
            Input::SetMouseMode(MouseMode::Locked);

            float deltaX = Input::GetMouseDeltaX();
            float deltaY = Input::GetMouseDeltaY();

            // Check if the mouse movement is non-zero
            if (deltaX != 0 || deltaY != 0)
            {
                // Update the rotation with sensitivity based on delta values
                float sensitivityX = deltaX * sensitivity;
                float sensitivityY = -deltaY * sensitivity;

                // Update rotation based on mouse movement
                camera->GetRotation()->y += sensitivityX;
                camera->GetRotation()->x += sensitivityY;

                camera->GetRotation()->x = Math::Clamp(Math::DegToRad(-89), Math::DegToRad(89), camera->GetRotation()->x);
            }
        }
        else
        {
            // If the right mouse button is not pressed, set the mouse mode to visible
            Input::SetMouseMode(MouseMode::Visible);
        }
    }

}