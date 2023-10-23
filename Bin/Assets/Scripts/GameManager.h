#pragma once

#include <Core.h>

enum GameState
{
    GamePlay,
    GameStop,
};

enum WinCases
{
    WinLeft,
    WinRight,
    Draw,
};

class GameManager : public Core::ActorScript
{
public:
    GameState state;
    WinCases winCase;

    GameManager();
    ~GameManager();

    void OnUpdate();

    void StartGame();
    void GameOver();
    void SetWinState(WinCases wCase);
};

CE_DEFINE_ACTOR_SCRIPT(GameManager);
