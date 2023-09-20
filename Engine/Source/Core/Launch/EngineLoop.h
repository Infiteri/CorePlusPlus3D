#pragma once

#include "Base.h"
#include "Core/Application.h"
#include "Core/CeConfigurations.h"



namespace Core
{
    /// @brief Wrapper around the engine class.
    class CE_API EngineLoop
    {
    public:
        EngineLoop();
        ~EngineLoop();

        void PreInit(Application* app,EngineConfiguration* config);

        void Init();

        void Tick();

        void Shutdown();

        bool ShouldTick();
    };
}