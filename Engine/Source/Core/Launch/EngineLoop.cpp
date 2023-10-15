#include "EngineLoop.h"
#include "Core/CeAssert.h"

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
        CE_ASSERT_IF(app == nullptr && "EngineLoop::PreInit: Application cannot be a nullptr.");      // Crash if app is nullptr
        CE_ASSERT_IF(config == nullptr && "EngineLoop::PreInit: Configuration cannot be a nullptr."); // Crash if config is nullptr

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