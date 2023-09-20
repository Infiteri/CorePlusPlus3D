#pragma once

#include "Base.h"

namespace Core
{
    enum class PlatformLogColor
    {
        White,
        Red,
        DarkRed,
        Blue,
        Gray,
        Green,
        Yellow,
    };

    class CE_API Platform
    {
    public:
        Platform(){};
        ~Platform(){};

        static void SetConsoleColor(PlatformLogColor color);
        static void LogMessage(const char *msg);

        static void *MemSet(void *block, int value, CeU64 size);
        static void Free(void *block);
        static void *Allocate(CeU64 size);
        static void *MemCpy(void *dest, const void *source, CeU64 size);
    };
    

}