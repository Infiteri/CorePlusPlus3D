#include "LoggerPanel.h"

#include "Core/Logger.h"

namespace Core
{

    LoggerPanel::LoggerPanel()
    {
    }

    LoggerPanel::~LoggerPanel()
    {
    }

    void LoggerPanel::OnImGuiRender()
    {
        ImGui::Begin("Logger");

        for (auto info : Logger::GetLogInfos())
        {
            ImVec4 color;

            switch (info.Level)
            {
            case LoggingLevel::Info:
                color = {0, 1, 0.5, 1};
                break;

            case LoggingLevel::Warn:
                color = {1, 1, 0, 1};
                break;

            case LoggingLevel::Error:
                color = {1, 0, 0, 1};
                break;

            case LoggingLevel::Fatal:
                color = {1, 0, 0, 1};
                break;

            case LoggingLevel::Trace:
                color = {0.9f, 0.9f, 0.9f, 1};
                break;

            case LoggingLevel::Debug:
                color = {0, 0.49, 1, 1};
                break;

            default:
                color = {1, 1, 1, 1};
                break;
            }

            ImGui::PushStyleColor(ImGuiCol_Text, color);
            ImGui::Text(info.Message.c_str());
            ImGui::PopStyleColor();
        }

        ImGui::End();
    }
}