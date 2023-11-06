#include "MaterialLoader.h"
#include "Core/Logger.h"
#include "Platform/FileSystem.h"

#include "Utils/StringUtils.h"

#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <locale>

namespace Core
{
    Color StringToColor(const std::string &value);

    MaterialConfiguration MaterialLoader::GetConfigFromFile(const std::string &path)
    {
        MaterialConfiguration config;
        config.name = path;

        FileHandle file;
        file.Open(path);

        if (!file.Valid())
        {
            CE_CORE_FATAL("Unable to open handle for reading: %s when loading material configuration.", path.c_str());
            return config;
        }

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
                else if (StringUtils::EqualI(key, "name"))
                {
                    CE_CORE_ERROR("MaterialLoader: Names are now the file path. The name will be the filepath.");
                }
                else if (StringUtils::EqualI(key, "colorTextureName"))
                {
                    config.colorTextureName = value;
                }
                else if (StringUtils::EqualI(key, "normalTextureName"))
                {
                    config.normalTextureName = value;
                }
                else if (StringUtils::EqualI(key, "color"))
                {
                    config.color = StringToColor(value);
                }
                else if (StringUtils::EqualI(key, "shininess"))
                {
                    config.shininess = std::stof(value);
                }
            }
        }

        return config;
    }

    Color StringToColor(const std::string &value)
    {
        std::istringstream colorStream(value);
        Color color;

        float r, g, b, a;
        colorStream >> r >> g >> b >> a;

        color.r = r;
        color.g = g;
        color.b = b;
        color.a = a;

        return color;
    }

}