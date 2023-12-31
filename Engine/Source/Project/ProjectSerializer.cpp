#include "ProjectSerializer.h"

#include <yaml-cpp/yaml.h>
#include <fstream>

namespace Core
{
    ProjectSerializer::ProjectSerializer(Project *proj)
    {
        project = proj;
    }

    ProjectSerializer::~ProjectSerializer()
    {
    }

    void ProjectSerializer::Serialize(const std::string &path)
    {
        CE_PROFILE_FUNCTION();

        auto config = Project::GetConfig();

        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "Project" << YAML::Value;
        out << YAML::BeginMap;

        out << YAML::Key << "Name" << YAML::Value << config->name.c_str();
        out << YAML::Key << "StartScene" << YAML::Value << config->startScene.c_str();
        out << YAML::Key << "AssetPath" << YAML::Value << config->assetPath.c_str();
        out << YAML::Key << "ScriptPath" << YAML::Value << config->scriptPath.c_str();
        out << YAML::Key << "ScriptFilesPath" << YAML::Value << config->scriptFilesPath.c_str();
        out << YAML::Key << "BuildScriptsPath" << YAML::Value << config->buildScriptsPath.c_str();

        out << YAML::EndMap;

        out << YAML::EndSeq;
        out << YAML::EndMap;

        std::ofstream fout(path);
        fout << out.c_str();
    }

    void ProjectSerializer::Deserialize(const std::string &path)
    {
        CE_PROFILE_FUNCTION();

        auto config = Project::GetConfig();

        YAML::Node data = YAML::LoadFile(path.c_str());

        auto projNode = data["Project"];
        if (!projNode)
            return;

        config->name = projNode["Name"].as<std::string>();
        config->startScene = projNode["StartScene"].as<std::string>();
        config->assetPath = projNode["AssetPath"].as<std::string>();
        config->scriptPath = projNode["ScriptPath"].as<std::string>();
        config->scriptFilesPath = projNode["ScriptFilesPath"].as<std::string>();
        config->buildScriptsPath = projNode["BuildScriptsPath"].as<std::string>();
    }
    void ProjectSerializer::DeserializeOrMakeFileIfNotExistent(const std::string &path)
    {
        CE_PROFILE_FUNCTION();

        try
        {
            Deserialize(path);
        }
        catch (const std::exception &)
        {
            auto config = Project::GetConfig();
            config->name = "DefaultProject";
            config->startScene = "DefaultScene";
            config->assetPath = "Assets";
            config->scriptPath = "Scripts";
            config->scriptFilesPath = "ScriptFiles";
            config->buildScriptsPath = "BuildScripts";

            // Serialize the default configuration to the specified path
            Serialize(path);

            // Close the file before attempting to deserialize again
            std::ifstream file(path);
            if (file.is_open())
                file.close();

            // Deserialize the defaults
            Deserialize(path);
        }
    }

}