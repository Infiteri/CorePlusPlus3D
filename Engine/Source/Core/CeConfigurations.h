#pragma once

#include "Base.h"

namespace Core
{
    struct WindowConfiguration
    {
        int x;
        int y;
        int width;
        int height;
        const char *title;
        bool startMaximized = false;
    };

    struct EngineConfiguration
    {
        WindowConfiguration windowConfig;
    };
}