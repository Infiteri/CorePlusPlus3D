#include "EngineLoop.h"

#include "Core/Engine.h"
#include "Core/CeConfigurations.h"

namespace Core
{
    EngineLoop::EngineLoop()
    {
    }

    EngineLoop::~EngineLoop()
    {
    }

    void EngineLoop::PreInit(Application *app, EngineConfiguration *config)
    {
        Engine::PreInit(app, config);
    }

    void EngineLoop::Init()
    {
        Engine::Init();

        Engine::Start();
    }

    void EngineLoop::Tick()
    {
        Engine::Update();

        Engine::Render();
    }

    void EngineLoop::Shutdown()
    {
        Engine::Shutdown();
    }

    bool EngineLoop::ShouldTick()
    {
        return Engine::ShouldTick();
    }
}