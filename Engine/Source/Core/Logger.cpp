#include "Logger.h"

#include "Platform/Platform.h"

#include <cstdarg>
#include <unordered_map>

namespace Core
{
    static std::unordered_map<LoggingLevel, std::string> levelToStringMap;
    static std::unordered_map<std::string, LogCategory *> categories;
    static std::vector<LogInfo> logs;
    static LoggerInformation info;

    Logger::Logger()
    {
    }

    Logger::~Logger()
    {
    }

    void Logger::Init(LoggerInformation information)
    {
        info = information;

        CE_PROFILE_FUNCTION();

        DefineLogCategory("Core", CE_CORE_LOGGER_NAME);
        DefineLogCategory("Client", CE_CLIENT_LOGGER_NAME);

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

    void Logger::Log(LoggingLevel level, const std::string &category, const char *fmt, ...)
    {
        if (level == LoggingLevel::Error || level == LoggingLevel::Fatal)
        {
        }
        else
        {
            for (auto s : info.IgnoredCategories)
            {
                if (s == category) // Ignored category.
                    return;
            }
        }

        // The actual string representation of the level
        std::string logDescriptionString = levelToStringMap[level];
        auto cat = categories[category];

        if (!cat)
            DefineLogCategory(category.c_str(), category);

        // TODO: Refactor strings
        char OutMessage[CE_LOGGER_MAX_MESSAGE_LENGTH];
        Platform::MemSet(OutMessage, 0, CE_LOGGER_MAX_MESSAGE_LENGTH);

        // Perform string pending
        __builtin_va_list arg_ptr;
        va_start(arg_ptr, fmt);
        vsnprintf(OutMessage, CE_LOGGER_MAX_MESSAGE_LENGTH, fmt, arg_ptr);
        va_end(arg_ptr);

        char OutMessageWithLevels[CE_LOGGER_MAX_MESSAGE_LENGTH];
        Platform::MemSet(OutMessageWithLevels, 0, CE_LOGGER_MAX_MESSAGE_LENGTH);

        if (info.PendLevelToOuput)
            snprintf(OutMessageWithLevels, CE_LOGGER_MAX_MESSAGE_LENGTH, "[%s %s]: %s\n", cat->Pending, logDescriptionString.c_str(), OutMessage);
        else
            snprintf(OutMessageWithLevels, CE_LOGGER_MAX_MESSAGE_LENGTH, "[%s]: %s\n", cat->Pending, OutMessage);

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

        // Push a log info
        LogInfo inf;
        inf.Level = level;
        inf.Message = OutMessageWithLevels;
        inf.Pending = cat->Pending;
        logs.push_back(inf);
    }

    std::vector<LogInfo> Logger::GetLogInfos()
    {
        return logs;
    }

    void Logger::ClearLogInfos()
    {
        logs.clear();
    }

    void Logger::DefineLogCategory(const char *pending, const std::string &name)
    {
        categories[name] = new LogCategory;
        categories[name]->Pending = pending;
    }
}