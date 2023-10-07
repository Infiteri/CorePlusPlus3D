#pragma once

#include "Base.h"

namespace Core
{
    struct CustomImageHeader
    {
        CeU32 width;
        CeU32 height;
        CeU32 channels; // Number of color channels (e.g., 3 for RGB)
        // Add more fields as needed
    };

    struct CustomImageFile
    {
        CustomImageHeader header;
        CeU8 *pixelData; // Pointer to pixel data
    };
    class CustomImageLoader
    {
    public:
        CustomImageLoader(){};
        ~CustomImageLoader(){};

        static char *Load(const char *path, int *w, int *h, int *channel);
    };
}