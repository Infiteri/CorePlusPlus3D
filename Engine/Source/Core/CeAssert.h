#pragma once

#include "Base.h"
#include "Logger.h"

#define CE_ENABLE_ASSERTIONS 1

#define CeBreakDebugger __builtin_trap

#if CE_ENABLE_ASSERTIONS == 1

#define CE_ASSERT_NOT(cond)                                                          \
    if (!(cond))                                                                     \
    {                                                                                \
        CE_FATAL("Assertion failure: '%s'. (%s at %i).", #cond, __FILE__, __LINE__); \
        CeBreakDebugger();                                                           \
    }

#define CE_ASSERT_IF(cond)                                                           \
    if (cond)                                                                        \
    {                                                                                \
        CE_FATAL("Assertion failure: '%s'. (%s at %i).", #cond, __FILE__, __LINE__); \
        CeBreakDebugger();                                                           \
    }

#else
#define CE_ASSERT_NOT(cond)
#define CE_ASSERT_IF(cond)
#endif
