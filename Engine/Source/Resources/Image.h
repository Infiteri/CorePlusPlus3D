#pragma once

#include "Base.h"

#include <string>

namespace Core
{
    class CE_API Image
    {
    private:
        std::string path;

        CeU8 *data;
        int width;
        int height;
        int channels;
        bool valid = false;

    public:
        Image(const std::string &path);
        ~Image();

        CeU8 *GetData();
        int GetWidth();
        int GetHeight();
        int GetChannels();
        std::string GetPath();

        bool IsValid();

        void FreeData();
    };

}
