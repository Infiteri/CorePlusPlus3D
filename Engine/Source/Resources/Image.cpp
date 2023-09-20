#include "Image.h"
#include "Core/Logger.h"

// STB Image
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// End STB Image

namespace Core
{
    Image::Image(const std::string &_path)
    {
        this->path = _path;

        data = 0;
        width = 0;
        height = 0;
        channels = 0;
        valid = false;

        data = stbi_load(path.c_str(), &width, &height, &channels, 0);

        if (!data)
        {
            valid = false;
            const char *reason = stbi_failure_reason();
            CE_FATAL("Image loading failure: STB Reason: %s (%s)", reason, path.c_str());
            return;
        }

        valid = true;
    }

    Image::~Image()
    {
        stbi_image_free(data);
    }

    CeU8 *Image::GetData()
    {
        return data;
    }

    int Image::GetWidth()
    {
        return width;
    }

    int Image::GetHeight()
    {
        return height;
    }

    int Image::GetChannels()
    {
        return channels;
    }

    std::string Image::GetPath()
    {
        return path;
    }
}