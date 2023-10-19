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
        auto config = Project::GetConfig();

        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "Project" << YAML::Value;
        out << YAML::BeginMap;

        out << YAML::Key << "Name" << YAML::Value << config->name.c_str();
        out << YAML::Key << "StartScene" << YAML::Value << config->startScene.c_str();
        out << YAML::Key << "AssetPath" << YAML::Value << config->assetPath.c_str();
        out << YAML::Key << "ScriptPath" << YAML::Value << config->scriptPath.c_str();

        out << YAML::EndMap;

        out << YAML::EndSeq;
        out << YAML::EndMap;

        std::ofstream fout(path);
        fout << out.c_str();
    }

    void ProjectSerializer::Deserialize(const std::string &path)
    {
        auto config = Project::GetConfig();

        YAML::Node data = YAML::LoadFile(path.c_str());

        auto projNode = data["Project"];
        if (!projNode)
            return;

        config->name = projNode["Name"].as<std::string>();
        config->startScene = projNode["StartScene"].as<std::string>();
        config->assetPath = projNode["AssetPath"].as<std::string>();
        config->scriptPath = projNode["ScriptPath"].as<std::string>();
    }
}