#include <PlayerScript.h>

PlayerScript::PlayerScript() {}
PlayerScript::~PlayerScript() {}

void PlayerScript::OnUpdate()
{
    auto transform = GetTransform();

    if (!transform)
        return;

    if (Core::Input::GetKey(Core::Keys::W))
    {
        transform->GetPosition()->y += moveSpeed;
    }

    if (Core::Input::GetKey(Core::Keys::S))
    {
        transform->GetPosition()->y -= moveSpeed;
    }
}
