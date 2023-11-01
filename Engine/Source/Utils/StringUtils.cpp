#include "StringUtils.h"
#include <algorithm>
#include <sstream>

namespace Core
{
    namespace StringUtils
    {
        std::vector<std::string> SplitString(const std::string &input, char delimiter)
        {
            std::vector<std::string> result;
            std::stringstream ss(input);
            std::string item;
            while (std::getline(ss, item, delimiter))
            {
                result.push_back(item);
            }
            return result;
        }

        std::string Trim(const std::string &input)
        {
            const std::string whitespaces = " \t\r\n";
            size_t start = input.find_first_not_of(whitespaces);
            if (start == std::string::npos)
                return "";

            size_t end = input.find_last_not_of(whitespaces);
            return input.substr(start, end - start + 1);
        }

        std::string ToLowerCase(const std::string &input)
        {
            std::string result = input;
            std::transform(result.begin(), result.end(), result.begin(), ::tolower);
            return result;
        }

        std::string ToUpperCase(const std::string &input)
        {
            std::string result = input;
            std::transform(result.begin(), result.end(), result.begin(), ::toupper);
            return result;
        }

        std::string ReplaceAll(const std::string &input, const std::string &search, const std::string &replace)
        {
            std::string result = input;
            size_t pos = 0;
            while ((pos = result.find(search, pos)) != std::string::npos)
            {
                result.replace(pos, search.length(), replace);
                pos += replace.length();
            }
            return result;
        }

        std::string JoinString(const std::vector<std::string> &elements, const std::string &delimiter)
        {
            std::string result;
            for (size_t i = 0; i < elements.size(); ++i)
            {
                result += elements[i];
                if (i != elements.size() - 1)
                {
                    result += delimiter;
                }
            }
            return result;
        }

        bool StartsWith(const std::string &input, const std::string &prefix)
        {
            return input.rfind(prefix, 0) == 0;
        }

        bool EndsWith(const std::string &input, const std::string &suffix)
        {
            if (input.length() < suffix.length())
                return false;
            return input.compare(input.length() - suffix.length(), suffix.length(), suffix) == 0;
        }

        std::string RemoveCharacter(const std::string &input, char character)
        {
            std::string result = input;
            result.erase(std::remove(result.begin(), result.end(), character), result.end());
            return result;
        }

        std::string FormatFilePath(const std::string &filePath)
        {
            std::string formattedPath = filePath;
            std::replace(formattedPath.begin(), formattedPath.end(), '\\', '/');
            return formattedPath;
        }

        std::string GetFileExtension(const std::string &filePath)
        {
            size_t dotPos = filePath.rfind('.');
            if (dotPos != std::string::npos && dotPos < filePath.length() - 1)
            {
                return filePath.substr(dotPos + 1);
            }
            return "";
        }

        std::string GetFileName(const std::string &filePath)
        {
            size_t lastSlash = filePath.find_last_of("/\\"); // Find the last slash or backslash

            if (lastSlash != std::string::npos)
            {
                return filePath.substr(lastSlash + 1);
            }
            else
            {
                return filePath;
            }
        }

        std::string RemoveFileExtension(const std::string &filePath)

        {
            size_t dotPos = filePath.rfind('.');
            if (dotPos != std::string::npos)
            {
                return filePath.substr(0, dotPos);
            }
            return filePath;
        }

        bool Equal(const std::string &a, const std::string &b)
        {

            return a == b;
        }

        bool EqualI(const std::string &a, const std::string &b)
        {
            if (a.length() != b.length())
                return false;

            // Convert both strings to lowercase and then compare
            std::string lowercaseA = a;
            std::string lowercaseB = b;

            // Convert both strings to lowercase using std::tolower
            std::transform(lowercaseA.begin(), lowercaseA.end(), lowercaseA.begin(), ::tolower);
            std::transform(lowercaseB.begin(), lowercaseB.end(), lowercaseB.begin(), ::tolower);

            return lowercaseA == lowercaseB;
        }

        bool FileExtIsImage(const std::string &fullFileName)
        {
            std::string ext = GetFileExtension(fullFileName);
            return (ext == "png" || ext == "jpg" || ext == "jpeg" || ext == "ce_image");
        }
    }
}
