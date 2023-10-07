#include "CustomImageLoader.h"
#include "Core/Logger.h"

#include <iostream>
#include <fstream>

namespace Core
{

    char *CustomImageLoader::Load(const char *path, int *w, int *h, int *channels)
    {
        std::ifstream inputFile(path, std::ios::binary);
        if (!inputFile)
        {
            CE_ERROR("Failed to open custom image file for reading '%s'.", path);
            return nullptr;
        }

        // Read the header
        CustomImageHeader imageHeader;
        inputFile.read(reinterpret_cast<char *>(&imageHeader), sizeof(CustomImageHeader));

        *w = imageHeader.width;
        *h = imageHeader.height;
        *channels = imageHeader.channels;

        // Calculate the size of the pixel data
        CeU64 imageSize = *w * *h * *channels;

        // Allocate memory for pixel data
        char *imageData = new char[imageSize];

        // Read pixel data
        inputFile.read(imageData, imageSize);

        inputFile.close();

        return imageData;
    }

}