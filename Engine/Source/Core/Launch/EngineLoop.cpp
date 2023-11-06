#include "EngineLoop.h"
#include "Core/CeAssert.h"

#include "Core/Engine.h"
#include "Core/CeConfigurations.h"
#include "Core/Instrumentation.h"

#include <GLFW/glfw3.h>

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
        if (Engine::GetWantedFPS() > 0)
        {
            double currentTime = glfwGetTime();
            double deltaTime = currentTime - lastFrameTime;

            if (deltaTime < (1.0 / Engine::GetWantedFPS()))
            {
                double remainingTime = (1.0 / Engine::GetWantedFPS()) - deltaTime;
                glfwWaitEventsTimeout(remainingTime); 
                currentTime = glfwGetTime();
            }
            lastFrameTime = currentTime;
        }

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