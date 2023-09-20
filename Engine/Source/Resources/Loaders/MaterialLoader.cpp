#include "MaterialLoader.h"
#include "Core/Logger.h"
#include "Platform/FileSystem.h"

#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <locale>

namespace Core
{
    std::string toLowerCase(const std::string &str)
    {
        std::string result = str;
        for (char &c : result)
        {
            if (c >= 'A' && c <= 'Z')
            {
                // Convert uppercase letter to lowercase
                c = c - 'A' + 'a';
            }
        }
        return result;
    }

    MaterialConfiguration MaterialLoader::GetConfigFromFile(const std::string &path)
    {
        MaterialConfiguration config;

        FileHandle file;
        file.Open(path);

        if (!file.Valid())
        {
            CE_FATAL("Unable to open handle for reading: %s when loading material configuration.", path.c_str());
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

                // TODO: FILL PLS
                key = toLowerCase(key);
                value = toLowerCase(value);
            }
        }

        return config;
    }

}