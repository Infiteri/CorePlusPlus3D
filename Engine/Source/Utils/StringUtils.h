#pragma once

#include "Base.h"

#include <string>
#include <vector>

namespace Core
{
    namespace StringUtils
    {
        // Splits a string into multiple substrings based on a delimiter
        CE_API std::vector<std::string> SplitString(const std::string &input, char delimiter);

        // Removes leading and trailing whitespaces from a string
        CE_API std::string Trim(const std::string &input);

        // Converts a string to lowercase
        CE_API std::string ToLowerCase(const std::string &input);

        // Converts a string to uppercase
        CE_API std::string ToUpperCase(const std::string &input);

        // Replaces all occurrences of a substring with another substring
        CE_API std::string ReplaceAll(const std::string &input, const std::string &search, const std::string &replace);

        // Joins elements of a string vector into a single string with a delimiter
        CE_API std::string JoinString(const std::vector<std::string> &elements, const std::string &delimiter);

        // Checks if a string starts with a given prefix
        CE_API bool StartsWith(const std::string &input, const std::string &prefix);

        // Checks if a string ends with a given suffix
        CE_API bool EndsWith(const std::string &input, const std::string &suffix);

        // Removes all occurrences of a character from a string
        CE_API std::string RemoveCharacter(const std::string &input, char character);

        // Ensures a file path has the correct format based on the operating system (e.g., replaces '\' with '/' on Windows)
        CE_API std::string FormatFilePath(const std::string &filePath);

        // Extracts the file extension from a file path
        CE_API std::string GetFileExtension(const std::string &filePath);

        CE_API std::string GetFileName(const std::string &filePath);


        // Removes the file extension from a file path
        CE_API std::string RemoveFileExtension(const std::string &filePath);

        CE_API bool Equal(const std::string &a, const std::string &b);
        CE_API bool EqualI(const std::string &a, const std::string &b);

        CE_API bool FileExtIsImage(const std::string& fullFileName);
    };
}