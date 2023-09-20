#pragma once

#include "Base.h"
#include "Logger.h"

#define CeBreakDebugger __builtin_trap

#define CE_ASSERT(cond)                                                              \
    if (cond)                                                                        \
    {                                                                                \
    }                                                                                \
    else                                                                             \
    {                                                                                \
        CE_FATAL("Assertion failure: '%s'. (%s at %i).", #cond, __FILE__, __LINE__); \
        CeBreakDebugger();                                                           \
    }
