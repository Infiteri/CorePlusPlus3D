#pragma once

#include "EngineLoop.h"
#include "Core/Application.h"
#include "Platform/Platform.h"

extern Core::Application *Core::CreateApplication(Core::EngineConfiguration *config);

int main(int argc, char const *argv[])
{
    Core::EngineConfiguration config;

    // Setup default config
    Core::Platform::MemSet(&config, 0, sizeof(config));
    config.windowConfig.x = 100;
    config.windowConfig.y = 100;
    config.windowConfig.width = 1024;
    config.windowConfig.height = 576;
    config.windowConfig.title = "Hello Core Engine";

    Core::EngineLoop GEngineLoop;

    Core::Application *application = Core::CreateApplication(&config);

    // pre Init stage
    GEngineLoop.PreInit(application, &config);

    // Init stage
    GEngineLoop.Init();

    // Tick stage
    while (GEngineLoop.ShouldTick())
    {
        GEngineLoop.Tick();
    }

    // Exit / Shutdown stage
    GEngineLoop.Shutdown();

    return 0;
}
