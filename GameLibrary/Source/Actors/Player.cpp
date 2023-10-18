#include "Player.h"

class CustomComponent : public Core::Component
{
public:
    CustomComponent()
    {
        custom = true;
    };
    ~CustomComponent();

    void OnImGuiRender(){};
};

void PlayerScript::OnUpdate()
{
    GetTransform()->GetRotation()->y += Core::Math::DegToRad(1);
}

void PlayerScript::OnStart()
{
    auto component = AddComponent<CustomComponent>();
}
