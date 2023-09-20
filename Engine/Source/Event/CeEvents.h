#pragma once

#include "Base.h"
#include "Core/Input.h"

namespace Core
{
    enum class MouseEventType
    {
        Press,
        Release
    };

    enum class KeyEventType
    {
        Press,
        Repeat,
        Release
    };

    struct MouseMoveEvent
    {
        CeU32 x;
        CeU32 y;
    };

    struct KeyEvent
    {
        Keys key;
        KeyEventType type;
    };

    struct MouseClickEvent
    {
        CeU32 x;
        CeU32 y;
        Buttons button;
        MouseEventType type;
    };

    struct ResizeEvent
    {
        int width;
        int height;
    };
}