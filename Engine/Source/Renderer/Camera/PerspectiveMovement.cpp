#include "PerspectiveMovement.h"

#include "Core/Input.h"

namespace Core
{
    PerspectiveMovement::PerspectiveMovement()
    {
        speed = 0.02f;
        rotation = 0.01f;
        sensitivity = 0.001f;
    }

    PerspectiveMovement::~PerspectiveMovement()
    {
    }

    void PerspectiveMovement::Update(PerspectiveCamera *camera)
    {
        if (Input::GetKey(Keys::Left))
        {
            camera->GetRotation()->y -= rotation;
        }

        if (Input::GetKey(Keys::Right))
        {
            camera->GetRotation()->y += rotation;
        }

        if (Input::GetKey(Keys::Up))
        {
            camera->GetRotation()->x -= rotation;
        }

        if (Input::GetKey(Keys::Down))
        {
            camera->GetRotation()->x += rotation;
        }

        // float deltaX = Input::GetMouseDeltaX();
        // float deltaY = Input::GetMouseDeltaY();
        // if (deltaX != 0 || deltaY != 0)
        // {
        //     camera->GetRotation()->y += deltaX * sensitivity;
        // }

        if (Input::GetKey(Keys::W))
        {
            Matrix4 cameraMatrixRotation = Matrix4::RotationXYZ(camera->GetRotation());
            Vector3 way = Matrix4::Forward(&cameraMatrixRotation);
            camera->GetPosition()->x += way.x * speed;
            camera->GetPosition()->y += way.y * speed;
            camera->GetPosition()->z += way.z * speed;
        }

        if (Input::GetKey(Keys::S))
        {
            Matrix4 cameraMatrixRotation = Matrix4::RotationXYZ(camera->GetRotation());
            Vector3 way = Matrix4::Forward(&cameraMatrixRotation);
            camera->GetPosition()->x -= way.x * speed;
            camera->GetPosition()->y -= way.y * speed;
            camera->GetPosition()->z -= way.z * speed;
        }

        if (Input::GetKey(Keys::A))
        {
            Matrix4 cameraMatrixRotation = Matrix4::RotationXYZ(camera->GetRotation());
            Vector3 way = Matrix4::Right(&cameraMatrixRotation);
            camera->GetPosition()->x -= way.x * speed;
            camera->GetPosition()->y -= way.y * speed;
            camera->GetPosition()->z -= way.z * speed;
        }

        if (Input::GetKey(Keys::D))
        {
            Matrix4 cameraMatrixRotation = Matrix4::RotationXYZ(camera->GetRotation());
            Vector3 way = Matrix4::Right(&cameraMatrixRotation);
            camera->GetPosition()->x += way.x * speed;
            camera->GetPosition()->y += way.y * speed;
            camera->GetPosition()->z += way.z * speed;
        }
    }
}