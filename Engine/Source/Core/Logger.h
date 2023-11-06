#pragma once

//? ------------------------------ Welcome to Core/Logger.h ------------------------------
//? This serves as a little explanation for how logging works right now.
//? Last Update:
//?                 06 November 2023. 3:10 PM. (As of writing this)
//? --------------------------------------------------------------------------------------
//? Pendings are the string before the level:
//? "Core Warn" - The pending is "Core", the level is "Warn".
//
//? Logging is separated in two parts (for now), Core Logging and Client Logging (In the future we will have things like category logs which allow for custom pendings like "TextureManager (LEVEL): Texture found")
//
//? Any logging that contains with "_CORE_" in the middle is a part of Core Logging, with the pending of "Core". These logs are turned off if the CE_CORE_CAN_LOG is set to 0. The ERROR and FATAL logs are
//? NOT turned of.
//
//? Any logging that contains with "_CLIENT_" in the middle is a part of Client Logging, with the pending of "Client". These logs are turned off if the CE_CLIENT_CAN_LOG is set to 0.
//? The ERROR and FATAL logs are NOT turned off.

//? EXAMPLE
//
//? CE_CORE_INFO("Hello.") -> "Core Info: Hello."
//? #define CE_CORE_CAN_LOG 0
//? CE_CORE_INFO("Hello.") -> "" (NO LOG)
//
//
//? CE_INFO("Hello.") -> "Client Info: Hello."
//? #define CE_CLIENT_CAN_LOG 0
//? CE_INFO("Hello.") -> "" (NO LOG)
//
//? Keep in mind that setting these values are not enough, a re-compile of the whole engine is needed, something might be done about this later.
//? --------------------------------------------------------------------------------------

#include "Base.h"
#include "CeAssert.h"

#include <string>
#include <vector>

#define CE_ENABLE_INFO_LOGGING 1
#define CE_ENABLE_WARN_LOGGING 1
#define CE_ENABLE_TRACE_LOGGING 1
#define CE_ENABLE_DEBUG_LOGGING 1
#define CE_LOGGER_MAX_MESSAGE_LENGTH 32000

#define CE_CORE_CAN_LOG 0
#define CE_CLIENT_CAN_LOG 1

#define CE_CORE_LOGGER_NAME "__CoreLogger__"
#define CE_CLIENT_LOGGER_NAME "__ClientLogger__"

#ifdef CE_WITH_EDITOR
#include <imgui.h>
#endif

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

    struct LogInfo
    {
        std::string Message;
        LoggingLevel Level;
    };

    struct LogCategory
    {
        const char *Pending;
    };

    class CE_API Logger
    {
    public:
        Logger();
        ~Logger();

        static void Init();
        static void Shutdown();

        static void Log(LoggingLevel level, const std::string &category, const char *fmt, ...);

        static std::vector<LogInfo> GetLogInfos();
        static void ClearLogInfos();

        static void DefineLogCategory(const char *pending, const std::string &name);

        static void OnLoggerLogGeneral()
        {
#ifdef CE_WITH_EDITOR
            ImGui::SetScrollHereY(1.0f);
#endif
        };
    };
}

// !!! ALWAYS DEFINED

#define CE_CORE_ERROR(fmt, ...) Core::Logger::Log(Core::LoggingLevel::Error, CE_CORE_LOGGER_NAME, fmt, ##__VA_ARGS__)
#define CE_CORE_FATAL(fmt, ...) Core::Logger::Log(Core::LoggingLevel::Fatal, CE_CORE_LOGGER_NAME, fmt, ##__VA_ARGS__)

#define CE_ERROR(fmt, ...) Core::Logger::Log(Core::LoggingLevel::Error, CE_CLIENT_LOGGER_NAME, fmt, ##__VA_ARGS__)
#define CE_FATAL(fmt, ...) Core::Logger::Log(Core::LoggingLevel::Fatal, CE_CLIENT_LOGGER_NAME, fmt, ##__VA_ARGS__)

#if CE_CORE_CAN_LOG == 1

#ifndef CE_DIST_BUILD
#if CE_ENABLE_INFO_LOGGING == 1
#define CE_CORE_INFO(fmt, ...) Core::Logger::Log(Core::LoggingLevel::Info, CE_CORE_LOGGER_NAME, fmt, ##__VA_ARGS__)
#else
#define CE_CORE_INFO(fmt, ...)
#endif

#if CE_ENABLE_WARN_LOGGING == 1
#define CE_CORE_WARN(fmt, ...) Core::Logger::Log(Core::LoggingLevel::Warn, CE_CORE_LOGGER_NAME, fmt, ##__VA_ARGS__)
#else
#define CE_CORE_WARN(fmt, ...)
#endif

#else

#define CE_CORE_INFO(fmt, ...)
#define CE_CORE_WARN(fmt, ...)
#endif
#else
#define CE_CORE_INFO(fmt, ...)
#define CE_CORE_WARN(fmt, ...)
#endif

// !!! PARTIALLY DEFINED
#if CE_CORE_CAN_LOG == 1
#if CE_DEBUG_MODE == 1
#if CE_ENABLE_TRACE_LOGGING == 1
#define CE_CORE_TRACE(fmt, ...) Core::Logger::Log(Core::LoggingLevel::Trace, CE_CORE_LOGGER_NAME, fmt, ##__VA_ARGS__)
#else
#define CE_CORE_TRACE(fmt, ...)
#endif

#if CE_ENABLE_TRACE_LOGGING == 1
#define CE_CORE_DEBUG(fmt, ...) Core::Logger::Log(Core::LoggingLevel::Debug, CE_CORE_LOGGER_NAME, fmt, ##__VA_ARGS__)
#else
#define CE_CORE_DEBUG(fmt, ...)
#endif

#else
#define CE_CORE_TRACE(fmt, ...)
#define CE_CORE_DEBUG(fmt, ...)
#endif
#else
#define CE_CORE_TRACE(fmt, ...)
#define CE_CORE_DEBUG(fmt, ...)
#endif

// NOTE: User

#if CE_CLIENT_CAN_LOG == 1
#ifndef CE_DIST_BUILD
#define CE_INFO(fmt, ...) Core::Logger::Log(Core::LoggingLevel::Info, CE_CLIENT_LOGGER_NAME, fmt, ##__VA_ARGS__)
#define CE_WARN(fmt, ...) Core::Logger::Log(Core::LoggingLevel::Warn, CE_CLIENT_LOGGER_NAME, fmt, ##__VA_ARGS__)
#else
#define CE_INFO(fmt, ...)
#define CE_WARN(fmt, ...)
#endif
#else
#define CE_INFO(fmt, ...)
#define CE_WARN(fmt, ...)
#endif

// !!! PARTIALLY DEFINED

#if CE_CLIENT_CAN_LOG == 1
#if CE_DEBUG_MODE == 1
#define CE_TRACE(fmt, ...) Core::Logger::Log(Core::LoggingLevel::Trace, CE_CLIENT_LOGGER_NAME, fmt, ##__VA_ARGS__)
#define CE_DEBUG(fmt, ...) Core::Logger::Log(Core::LoggingLevel::Debug, CE_CLIENT_LOGGER_NAME, fmt, ##__VA_ARGS__)
#else
#define CE_TRACE(fmt, ...)
#define CE_DEBUG(fmt, ...)
#endif
#else
#define CE_TRACE(fmt, ...)
#define CE_DEBUG(fmt, ...)
#endif