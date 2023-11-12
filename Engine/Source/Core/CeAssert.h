#pragma once

//? ------------------------------ Welcome to Core/CeAsserts.h --------------------------------
//? Every macro in this file are for use.
//? Assertions are disabled if its a distribution build or if the CE_ENABLE_ASSERTIONS flag is 0
//? A REBUILD IS REQUIRED IF THIS CHANGES !
//?
//? USE CASES:
//?     Keep in mind that if an assertion fails the WHOLE application will crash. ;)
//?
//? CE_ASSERT_IF(true); -> Will crash;
//? CE_ASSERT_NOT(true); -> Will not crash;
//? -------------------------------------------------------------------------------------------

#include "Base.h"
#include "Logger.h"
        
#define CE_ENABLE_ASSERTIONS 1

#define CeBreakDebugger __builtin_trap

#if CE_ENABLE_ASSERTIONS == 1

#define CE_ASSERT_NOT(cond)                                                               \
    if (!(cond))                                                                          \
    {                                                                                     \
        CE_CORE_FATAL("Assertion failure: '%s'. (%s at %i).", #cond, __FILE__, __LINE__); \
        CeBreakDebugger();                                                                \
    }

#define CE_ASSERT_IF(cond)                                                                \
    if (cond)                                                                             \
    {                                                                                     \
        CE_CORE_FATAL("Assertion failure: '%s'. (%s at %i).", #cond, __FILE__, __LINE__); \
        CeBreakDebugger();                                                                \
    }

#else
#define CE_ASSERT_NOT(cond)
#define CE_ASSERT_IF(cond)
#endif
