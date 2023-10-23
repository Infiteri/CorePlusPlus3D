#include <BallScript.h>

#include <GameManager.h>

void BallScript::OnStart()
{
    Core::Actor *left = GetActorByName("WallLeft");
    Core::Actor *right = GetActorByName("WallRight");
    Core::Actor *top = GetActorByName("WallTop");
    Core::Actor *bottom = GetActorByName("WallBottom");

    // Empty the aabb vector
    for (int i = 0; i < 3; i++)
        wallAABBs.push_back(nullptr);

    wallAABBs[0] = left->GetComponent<Core::AABBComponent>();
    wallAABBs[1] = right->GetComponent<Core::AABBComponent>();
    wallAABBs[2] = top->GetComponent<Core::AABBComponent>();
    wallAABBs[3] = bottom->GetComponent<Core::AABBComponent>();

    gameManagerScript = Core::ScriptEngine::GetScriptByName<GameManager>("GameManager");
}

void BallScript::OnUpdate()
{
    if (gameManagerScript->state == GameStop)
        return;

    auto transform = GetTransform();
    auto left = GetActorByName("Left"); // Left is the player
    auto right = GetActorByName("Right");
    auto aabb = GetComponent<Core::AABBComponent>();

    if (!transform || !left || !right || !aabb)
        return;

    auto rightAABB = right->GetComponent<Core::AABBComponent>();
    auto leftAABB = left->GetComponent<Core::AABBComponent>();

    if (aabb->CollidesWith(rightAABB))
    {
        velocity.y = Core::Math::RandomFloat(-0.5, 0.5);
        velocity.x = -velocity.x;
    }
    if (aabb->CollidesWith(leftAABB))
    {
        velocity.y = Core::Math::RandomFloat(-0.5, 0.5);
        velocity.x = -velocity.x;
    }

    // TODO: Player win state
    aabb->padding.x = (velocity.x < 0 ? -velocity.x : velocity.x) + 1;
    aabb->padding.y = (velocity.y < 0 ? -velocity.y : velocity.y) + 1;

    // left wall
    if (aabb->CollidesWith(wallAABBs[0]))
    {
        // Right wins
        // gameManagerScript->SetWinState(WinRight);
        // gameManagerScript->GameOver();
        velocity.x = -velocity.x;
        velocity.y = -velocity.y;
    }

    if (aabb->CollidesWith(wallAABBs[1]))
    {
        // Left wins
        // gameManagerScript->SetWinState(WinLeft);
        // gameManagerScript->GameOver();
        velocity.y = -velocity.y;
        velocity.x = -velocity.x;
    }

    if (aabb->CollidesWith(wallAABBs[2]))
    {
        velocity.y = -velocity.y;
    }

    if (aabb->CollidesWith(wallAABBs[3]))
    {
        velocity.y = -velocity.y;
    }

    transform->GetPosition()->x += velocity.x;
    transform->GetPosition()->y += velocity.y;
}

CE_IMPLEMENT_ACTOR_SCRIPT_DEFINE(BallScript);
