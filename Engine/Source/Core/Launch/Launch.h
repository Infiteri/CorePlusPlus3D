#pragma once

#include "EngineLoop.h"
#include "Core/Application.h"
#include "Platform/Platform.h"

extern Core::Application *Core::CreateApplication(Core::EngineConfiguration *config);

int main(int argc, char const *argv[])
{
    // Starts a profiling session when the engine starts.
    CE_PROFILE_BEGIN_SESSION_FILE("Game", "Debug/Core-Profile_Game.json");

    // Global configuration used when setting up the engine.
    Core::EngineConfiguration config;

    // Setup default config
    Core::Platform::MemSet(&config, 0, sizeof(config));
    config.windowConfig.x = 100;
    config.windowConfig.y = 100;
    config.windowConfig.width = 1024;
    config.windowConfig.height = 576;
    config.windowConfig.startMaximized = false;
    config.windowConfig.decorated = true;
    config.windowConfig.title = "Hello Core Engine";

    // Global engine loop manager
    Core::EngineLoop GEngineLoop;
    Core::Application *application = Core::CreateApplication(&config);

    // pre Init stage
    GEngineLoop.PreInit(application, &config);
    GEngineLoop.Init();
    // CE_PROFILE_END_SESSION();

    // Tick stage
    // CE_PROFILE_BEGIN_SESSION_FILE("Tick", "Debug/Core-Profile_Tick.json");
    while (GEngineLoop.ShouldTick())
    {
        GEngineLoop.Tick();
    }
    // CE_PROFILE_END_SESSION();

    // Exit / Shutdown stage
    // CE_PROFILE_BEGIN_SESSION_FILE("Shutdown", "Debug/Core-Profile_Tick.json");
    GEngineLoop.Shutdown();
    CE_PROFILE_END_SESSION();

    return 0;
}
