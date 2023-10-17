#pragma once

#include "Core.h"
#include "GameLibraryBase.h"

class GAME_API PlayerScript : public Core::ActorScript
{
public:
    void OnStart();
    void OnUpdate();
};

CE_EXPORT_ACTOR_SCRIPT(PlayerScript);