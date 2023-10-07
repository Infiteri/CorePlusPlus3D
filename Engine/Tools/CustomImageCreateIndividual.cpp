#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>

struct CustomImageHeader
{
    uint32_t width;
    uint32_t height;
    uint32_t channels; // Number of color channels (e.g., 3 for RGB)
    // Add more fields as needed
};

struct CustomImageFile
{
    CustomImageHeader header;
    unsigned char *pixelData; // Pointer to pixel data
};

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage: custom_image_creator <input_image_path> <output_image_path>" << std::endl;
        return 1;
    }

    const std::string inputImagePath = argv[1];
    const std::string outputImagePath = argv[2];

    int width, height, channels;
    unsigned char *imageData = stbi_load(inputImagePath.c_str(), &width, &height, &channels, 0);

    if (!imageData)
    {
        std::cerr << "Error: Failed to load input image: " << inputImagePath << std::endl;
        return 1;
    }

    CustomImageHeader imageHeader;
    imageHeader.width = width;
    imageHeader.height = height;
    imageHeader.channels = channels;

    std::ofstream outputFile(outputImagePath, std::ios::binary);
    if (!outputFile)
    {
        std::cerr << "Error: Failed to open output file for writing: " << outputImagePath << std::endl;
        stbi_image_free(imageData);
        return 1;
    }

    // Write the header and pixel data to the output file
    outputFile.write(reinterpret_cast<const char *>(&imageHeader), sizeof(CustomImageHeader));
    outputFile.write(reinterpret_cast<const char *>(imageData), width * height * channels);

    outputFile.close();
    stbi_image_free(imageData);

    std::cout << "Custom image created successfully: " << outputImagePath << std::endl;

    return 0;
}
