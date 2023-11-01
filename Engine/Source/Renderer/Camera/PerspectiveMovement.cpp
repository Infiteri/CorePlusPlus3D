#include "PerspectiveMovement.h"

#include "Core/Input.h"
#include "Core/Engine.h"

namespace Core
{
    PerspectiveMovement::PerspectiveMovement()
    {
        speed = 5.0f;
        rotation = 5.0f;
        sensitivity = 0.0f;
    }

    PerspectiveMovement::~PerspectiveMovement()
    {
    }

    void PerspectiveMovement::Update(PerspectiveCamera *camera)
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
}