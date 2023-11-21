#include "PlayerScript.h"

void PlayerScript::OnUpdate()
{
    if (Core::Input::GetKey(Core::Keys::W))
        velocity.y = speed;
    else if (Core::Input::GetKey(Core::Keys::S))
        velocity.y = -speed;
    else
        velocity.y = 0;

    GetTransform()->position.x += velocity.x;
    GetTransform()->position.y += velocity.y;
}

CE_IMPLEMENT_ACTOR_SCRIPT_DEFINE(PlayerScript);
