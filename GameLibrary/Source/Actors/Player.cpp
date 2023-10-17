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
}

void PlayerScript::OnStart()
{
    auto component = AddComponent<CustomComponent>();
}
