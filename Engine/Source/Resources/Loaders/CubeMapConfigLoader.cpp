#include "CubeMapConfigLoader.h"
#include "Core/Logger.h"
#include "Platform/FileSystem.h"

#include "Utils/StringUtils.h"
#include "Project/Project.h"

#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <locale>

namespace Core
{
    CubeMapConfiguration CubeMapConfigLoader::GetConfigFromFile(const std::string &path)
    {
        CubeMapConfiguration config;

        config.textureConfiguration.min = TextureFilter::Linear;
        config.textureConfiguration.mag = TextureFilter::Linear;

        FileHandle file;
        file.Open(path);

        if (!file.Valid())
        {
            CE_FATAL("Unable to open handle for reading: %s when loading cube map configuration.", path.c_str());
            return config;
        }

        ProjectConfiguration *pConfig = Project::GetConfig();

        std::string content = file.Read();

        // Split the content into lines
        std::istringstream contentStream(content);
        std::string line;
        std::vector<std::string> lines;

        while (std::getline(contentStream, line))
            lines.push_back(line);

        // Process each line
        for (const std::string &line : lines)
        {
            // Split the line into key and value
            size_t pos = line.find('=');
            if (pos != std::string::npos)
            {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                std::string prefix = pConfig ? pConfig->assetPath + "/" : "";

                // Trim leading and trailing whitespaces from key and value
                key.erase(0, key.find_first_not_of(" \t"));
                key.erase(key.find_last_not_of(" \t") + 1);
                value.erase(0, value.find_first_not_of(" \t"));
                value.erase(value.find_last_not_of(" \t") + 1);

                // WIP: FILL PLS
                key = StringUtils::ToLowerCase(key);

                if (StringUtils::EqualI(key, "version"))
                {
                    // TODO: Version check
                }
                else if (StringUtils::EqualI(key, "left"))
                {

                    config.left = prefix + value;
                }
                else if (StringUtils::EqualI(key, "right"))
                {

                    config.right = prefix + value;
                }
                else if (StringUtils::EqualI(key, "top"))
                {

                    config.top = prefix + value;
                }
                else if (StringUtils::EqualI(key, "bottom"))
                {

                    config.bottom = prefix + value;
                }
                else if (StringUtils::EqualI(key, "front"))
                {

                    config.front = prefix + value;
                }
                else if (StringUtils::EqualI(key, "back"))
                {

                    config.back = prefix + value;
                }
            }
        }

        return config;
    }
}