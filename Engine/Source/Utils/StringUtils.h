#pragma once

#include "Base.h"

#include <string>
#include <vector>

namespace Core
{
    namespace StringUtils
    {
        // Splits a string into multiple substrings based on a delimiter
        std::vector<std::string> SplitString(const std::string &input, char delimiter);

        // Removes leading and trailing whitespaces from a string
        std::string Trim(const std::string &input);

        // Converts a string to lowercase
        std::string ToLowerCase(const std::string &input);

        // Converts a string to uppercase
        std::string ToUpperCase(const std::string &input);

        // Replaces all occurrences of a substring with another substring
        std::string ReplaceAll(const std::string &input, const std::string &search, const std::string &replace);

        // Joins elements of a string vector into a single string with a delimiter
        std::string JoinString(const std::vector<std::string> &elements, const std::string &delimiter);

        // Checks if a string starts with a given prefix
        bool StartsWith(const std::string &input, const std::string &prefix);

        // Checks if a string ends with a given suffix
        bool EndsWith(const std::string &input, const std::string &suffix);

        // Removes all occurrences of a character from a string
        std::string RemoveCharacter(const std::string &input, char character);

        // Ensures a file path has the correct format based on the operating system (e.g., replaces '\' with '/' on Windows)
        std::string FormatFilePath(const std::string &filePath);

        // Extracts the file extension from a file path
        std::string GetFileExtension(const std::string &filePath);

        // Removes the file extension from a file path
        std::string RemoveFileExtension(const std::string &filePath);

        bool Equal(const std::string &a, const std::string &b);
        bool EqualI(const std::string &a, const std::string &b);
    };
}