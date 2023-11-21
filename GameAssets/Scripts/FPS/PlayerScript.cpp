#include "PlayerScript.h"

void PlayerScript::OnStart()
{
    Input::SetMouseMode(MouseMode::Locked);

    CameraActor = GetChildByName("Camera");
}

void PlayerScript::OnUpdate()
{
    auto tr = GetTransform();
    float dt = Core::Engine::GetDeltaTime();
    float FSpeed = Speed * dt;

    if (Input::GetKey(Keys::W))
        owner->MoveBy({FSpeed, 0, FSpeed});

    if (Input::GetKey(Keys::S))
        owner->MoveBy({-FSpeed, 0, -FSpeed});

    if (Input::GetKey(Keys::A))
        owner->MoveBy({FSpeed, 0, FSpeed});

    if (Input::GetKey(Keys::D))
        owner->MoveBy({-FSpeed, 0, -FSpeed});

    auto MouseDelta = Input::GetMouseDelta();

    if (MouseDelta.NotZero())
    {
        auto camTr = CameraActor->GetTransform();
        tr->rotation.y += MouseDelta.x * Sensitivity;
        camTr->rotation.x += -MouseDelta.y * Sensitivity;
        camTr->rotation.x = Math::Clamp(-(89 * CE_DEG_TO_RAD), 89 * CE_DEG_TO_RAD, camTr->rotation.x);
    }
}

CE_IMPLEMENT_ACTOR_SCRIPT_DEFINE(PlayerScript);
