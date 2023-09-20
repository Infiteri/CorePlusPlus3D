#include "FileSystem.h"

#include "Core/Logger.h"

namespace Core
{
    FileHandle::FileHandle()
    {
    }

    FileHandle::~FileHandle()
    {
    }

    bool FileHandle::Valid()
    {
        // Check if the file stream is open
        return stream.is_open();
    }

    void FileHandle::Open(const std::string &_path)
    {
        // Open the file at the specified path
        stream.open(_path);
        if (!stream.is_open())
        {
            CE_ERROR("Unable to open file: %s", _path.c_str());
        }
    }

    void FileHandle::Close()
    {
        stream.close();
    }

    std::string FileHandle::Read()
    {
        std::string fileContent;

        if (stream.is_open())
        {
            // Get the size of the file
            stream.seekg(0, std::ios::end);
            fileContent.resize(stream.tellg());
            stream.seekg(0, std::ios::beg);

            // Read the entire file into fileContent
            stream.read(&fileContent[0], fileContent.size());
            stream.close();
        }
        else
        {
            CE_ERROR("Unable to read file content");
            fileContent = "";
        }

        return fileContent;
    }
    std::string FileHandle::ReadLine(int lineNumber)
    {
        if (lineNumber < 0 || !stream.is_open())
        {
            return ""; // Return an empty string for invalid line numbers or if the file is not open
        }

        std::string line;
        for (int currentLine = 0; currentLine < lineNumber; ++currentLine)
        {
            if (!std::getline(stream, line))
            {
                return ""; // Return an empty string if we reach the end of the file before the desired line
            }
        }

        if (std::getline(stream, line))
        {
            return line; // Return the line we read
        }

        return ""; // Return an empty string if we can't read the line (e.g., end of file)
    }

    std::string FileSystem::ReadFileContent(const std::string &filename)
    {
        // Read files
        std::string vertResult;
        std::ifstream inVert(filename, std::ios::in | std::ios::binary);

        if (inVert)
        {

            inVert.seekg(0, std::ios::end);
            vertResult.resize(inVert.tellg());
            inVert.seekg(0, std::ios::beg);
            inVert.read(&vertResult[0], vertResult.size());
            inVert.close();
        }
        else
        {
            CE_ERROR("Unable to read file content:  %s", filename.c_str());
            vertResult = "";
        }

        return vertResult;
    }
}