#include "ActorScript.h"

namespace Core
{
    ActorScript::ActorScript()
    {
    }

    ActorScript::~ActorScript()
    {
    }

    void ActorScript::OnStart()
    {
    }

    void ActorScript::OnUpdate()
    {
    }

    void ActorScript::OnStop()
    {
    }
    Transform *ActorScript::GetTransform()
    {
        if (owner == nullptr)
            return nullptr;

        return owner->GetTransform();
    }
}
