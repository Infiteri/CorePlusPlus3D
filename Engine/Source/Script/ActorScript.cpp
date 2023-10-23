#include "ActorScript.h"
#include "Scene/World.h"

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

    Actor *ActorScript::GetActorByName(const std::string &name)
    {
        if (World::GetActive() == nullptr)
            return nullptr;

        return World::GetActive()->GetActorByName(name);
    }
}
