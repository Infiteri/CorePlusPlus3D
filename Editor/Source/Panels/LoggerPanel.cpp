#include "LoggerPanel.h"

#include "Core/Logger.h"
#include "Core/Memory/CeMemory.h"

#include <string>

namespace Core
{
    static int lastLogCount;
    static std::string NameString;
    static char Name[256];
    static std::string LevelToString = "All";

    LoggerPanel::LoggerPanel()
    {
        lastLogCount = Logger::GetLogInfos().size();
        CeMemory::Zero(&Name, 256);
    }

    LoggerPanel::~LoggerPanel()
    {
    }

    void LoggerPanel::OnImGuiRender()
    {
        ImGui::Begin("Logger");

        if (ImGui::InputText("Search value", Name, 256))
        {
            NameString = Name;
        }

        ImGui::SameLine();

        if (ImGui::Button("Clear"))
        {
            NameString = "";
            CeMemory::Zero(&Name, 256);
        }

        ImGui::SameLine();

        // -- DROP DOWN -----------
        const char *LevelToStringStrings[] = {"All", "Info", "Warn", "Error", "Fatal", "Trace", "Debug"};
        const int selCount = 7;
        if (ImGui::BeginCombo("Level", LevelToString.c_str()))
        {
            for (int i = 0; i < selCount; i++)
            {
                bool isSelected = (LevelToString == LevelToStringStrings[i]);
                if (ImGui::Selectable(LevelToStringStrings[i], isSelected))
                    LevelToString = LevelToStringStrings[i];
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        } // ------------------------

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

            if (LevelToString == LevelToStringStrings[1])
            {
                if (info.Level != LoggingLevel::Info)
                    continue;
            }

            if (LevelToString == LevelToStringStrings[2])
            {
                if (info.Level != LoggingLevel::Warn)
                    continue;
            }

            if (LevelToString == LevelToStringStrings[3])
            {
                if (info.Level != LoggingLevel::Error)
                    continue;
            }

            if (LevelToString == LevelToStringStrings[4])
            {
                if (info.Level != LoggingLevel::Fatal)
                    continue;
            }

            if (LevelToString == LevelToStringStrings[5])
            {
                if (info.Level != LoggingLevel::Trace)
                    continue;
            }

            if (LevelToString == LevelToStringStrings[6])
            {
                if (info.Level != LoggingLevel::Debug)
                    continue;
            }

            if (NameString.empty())
            {
                ImGui::PushStyleColor(ImGuiCol_Text, color);
                ImGui::Text(info.Message.c_str());
                ImGui::PopStyleColor();
            }
            else
            {
                if (info.Pending == NameString)
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, color);
                    ImGui::Text(info.Message.c_str());
                    ImGui::PopStyleColor();
                }
            }
        }

        if (lastLogCount != Logger::GetLogInfos().size())
        {
            lastLogCount = Logger::GetLogInfos().size();
            ImGui::SetScrollHereY(1.0f);
        }

        ImGui::End();
    }
}