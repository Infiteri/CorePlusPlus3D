#pragma once

#include "Base.h"

namespace Core
{
    /// @brief Basic window configuration.
    struct WindowConfiguration
    {
        /// @brief The start X position of the window.
        int x;

        /// @brief The start Y position of the window.
        int y;

        /// @brief The start width of the window.
        int width;

        /// @brief The start height of the window.
        int height;

        /// @brief The title window.
        const char *title;

        /// @brief Start maximized or not, if true, x, y, width and height are useless.
        bool startMaximized = false;

                /// @brief Use top bar.
        bool decorated = true;
    };

    /// @brief Setup engine configuration.
    struct EngineConfiguration
    {
        /// @brief Basic window configuration of the engine
        WindowConfiguration windowConfig;
    };
}