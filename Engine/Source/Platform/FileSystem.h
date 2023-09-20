#pragma once

#include "Base.h"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

namespace Core
{
    class FileHandle
    {
    private:
        std::ifstream stream;

    public:
        FileHandle();
        ~FileHandle();

        bool Valid();
        void Open(const std::string &_path);
        void Close();
        std::string Read();
        std::string ReadLine(int line);
    };

    class FileSystem
    {
    public:
        FileSystem(){};
        ~FileSystem(){};

        static std::string ReadFileContent(const std::string &filename);
    };

}