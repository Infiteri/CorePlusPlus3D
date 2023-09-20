#pragma once

#include "Base.h"
#include "CeConfigurations.h"

namespace Core
{
    class CE_API Application
    {
    public:
        Application();
        ~Application();

        virtual void Init();
        virtual void Shutdown();

        virtual void Update();
        virtual void Render();
    };

    Application *CreateApplication(EngineConfiguration *config);
}