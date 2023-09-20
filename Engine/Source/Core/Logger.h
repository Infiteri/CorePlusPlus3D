#pragma once

#include "Base.h"

namespace Core
{
    enum class LoggingLevel
    {
        Info,
        Warn,
        Error,
        Fatal,
        Trace,
        Debug,
    };

    class CE_API Logger
    {
    public:
        Logger();
        ~Logger();

        static void Init();
        static void Shutdown();

        static void Log(LoggingLevel level, const char *fmt, ...);
    };
}

// !!! ALWAYS DEFINED

#define CE_ERROR(fmt, ...) Core::Logger::Log(Core::LoggingLevel::Error, fmt, ##__VA_ARGS__)
#define CE_FATAL(fmt, ...) Core::Logger::Log(Core::LoggingLevel::Fatal, fmt, ##__VA_ARGS__)

// !!! PARTIALLY DEFINED

#ifndef CE_DIST_BUILD
#define CE_INFO(fmt, ...) Core::Logger::Log(Core::LoggingLevel::Info, fmt, ##__VA_ARGS__)
#define CE_WARN(fmt, ...) Core::Logger::Log(Core::LoggingLevel::Warn, fmt, ##__VA_ARGS__)
#else
#define CE_INFO(fmt, ...)
#define CE_WARN(fmt, ...)
#endif

// !!! PARTIALLY DEFINED

#ifdef CE_DEBUG_MODE
#define CE_TRACE(fmt, ...) Core::Logger::Log(Core::LoggingLevel::Trace, fmt, ##__VA_ARGS__)
#define CE_DEBUG(fmt, ...) Core::Logger::Log(Core::LoggingLevel::Debug, fmt, ##__VA_ARGS__)
#else
#define CE_TRACE(fmt, ...)
#define CE_DEBUG(fmt, ...)
#endif