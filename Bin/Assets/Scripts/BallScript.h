#pragma once

#include <Core.h>
#include <vector>

class GameManager;
class BallScript : public Core::ActorScript
{
public:
    Core::Vector2 velocity;
    std::vector<Core::AABBComponent *> wallAABBs;

    GameManager *gameManagerScript;

    BallScript(){};
    ~BallScript(){};

    void OnStart();
    void OnUpdate();
};

CE_DEFINE_ACTOR_SCRIPT(BallScript);