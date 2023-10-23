#include <GameManager.h>

GameManager::GameManager() {}

GameManager::~GameManager() {}

void GameManager::OnUpdate()
{
    switch (state)
    {
    case GamePlay:

        break;

    case GameStop:

        break;
    };
}

void GameManager::StartGame()
{
    state = GamePlay;
}

void GameManager::GameOver()
{
    state = GameStop;

    // Re-Setup the ball and player
    auto ball = GetActorByName("Ball");
    if (ball)
    {
        ball->GetTransform()->GetPosition()->Set(0, 0, 0);
    }

    auto left = GetActorByName("Left");
    if (left)
    {
        left->GetTransform()->GetPosition()->Set(-15, 0, 0);
    }

    auto right = GetActorByName("Right");
    if (right)
        right->GetTransform()->GetPosition()->Set(15, 0, 0);
}

void GameManager::SetWinState(WinCases wCase)
{
    winCase = wCase;
}

CE_IMPLEMENT_ACTOR_SCRIPT_DEFINE(GameManager);
