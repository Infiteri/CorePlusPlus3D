#include "Logger.h"

#include "Platform/Platform.h"

#include <cstdarg>
#include <unordered_map>
#include <string>

namespace Core
{
    static std::unordered_map<LoggingLevel, std::string> levelToStringMap;

    Logger::Logger()
    {
    }

    Logger::~Logger()
    {
    }

    void Logger::Init()
    {
        levelToStringMap[LoggingLevel::Info] = "Info";
        levelToStringMap[LoggingLevel::Warn] = "Warn";
        levelToStringMap[LoggingLevel::Error] = "Error";
        levelToStringMap[LoggingLevel::Fatal] = "Fatal";
        levelToStringMap[LoggingLevel::Trace] = "Trace";
        levelToStringMap[LoggingLevel::Debug] = "Debug";
    }

    void Logger::Shutdown()
    {
    }

    void Logger::Log(LoggingLevel level, const char *fmt, ...)
    {
        // The actual string representation of the level
        std::string logDescriptionString = levelToStringMap[level];

        // TODO: Refactor strings

        char OutMessage[32000];
        Platform::MemSet(OutMessage, 0, 32000);

        // Perform string pending
        __builtin_va_list arg_ptr;
        va_start(arg_ptr, fmt);
        vsnprintf(OutMessage, 32000, fmt, arg_ptr);
        va_end(arg_ptr);

        char OutMessageWithLevels[32000];
        Platform::MemSet(OutMessageWithLevels, 0, 32000);
        snprintf(OutMessageWithLevels, 32000, "[%s %s]: %s\n", "Core", logDescriptionString.c_str(), OutMessage);

        // WIP: Console colors
        PlatformLogColor color;
        switch (level)
        {
        case LoggingLevel::Info:
            color = PlatformLogColor::Green;
            break;

        case LoggingLevel::Warn:
            color = PlatformLogColor::Yellow;
            break;

        case LoggingLevel::Error:
            color = PlatformLogColor::Red;
            break;

        case LoggingLevel::Fatal:
            color = PlatformLogColor::DarkRed;
            break;

        case LoggingLevel::Trace:
            color = PlatformLogColor::Gray;
            break;

        case LoggingLevel::Debug:
            color = PlatformLogColor::Blue;
            break;

        default:
            color = PlatformLogColor::White;
            break;
        }

        // Done: Platform code
        Platform::SetConsoleColor(color);
        Platform::LogMessage(OutMessageWithLevels);
    }
}