#pragma once

#include "Base.h"
#include "Core/Scope.h"

#include <string>

namespace Core
{
    struct ProjectConfiguration
    {
        std::string name = "Project";
        std::string assetPath;
        std::string scriptPath;
        std::string scriptFilesPath;
        std::string buildScriptsPath;
        std::string startScene;
    };

    class CE_API Project
    {
    public:
        Project();
        ~Project();

        // -- STATIC --

        static Project *New();
        static Project *Load(const std::string &path);
        static void SaveActive(const std::string &path);

        static ProjectConfiguration *GetConfig();

        // --------

    private:
        ProjectConfiguration config;
    };
}