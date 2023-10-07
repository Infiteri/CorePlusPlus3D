#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <string>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

void PrintUse()
{
    std::cout << "Custom Image Creator" << std::endl;
    std::cout << "Usage: custom_image_creator <input_directory> <output_directory>" << std::endl;
    std::cout << "Process all image files in the input directory and create custom ce_img files in the output directory." << std::endl;
}

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

void ProcessDirectory(const std::string &inputDirPath, const std::string &outputDirPath);

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        PrintUse();
        return 1;
    }

    const std::string inputDirPath = argv[1];
    const std::string outputDirPath = argv[2];

    // Create the output directory if it doesn't exist
    if (mkdir(outputDirPath.c_str()) != 0 && errno != EEXIST)
    {
        std::cerr << "Error: Failed to create output directory: " << outputDirPath << std::endl;
        return 1;
    }

    ProcessDirectory(inputDirPath, outputDirPath);

    return 0;
}

void ProcessDirectory(const std::string &inputDirPath, const std::string &outputDirPath)
{
    DIR *dir;
    if ((dir = opendir(inputDirPath.c_str())) == nullptr)
    {
        std::cerr << "Error: Failed to open input directory." << std::endl;
        return;
    }

    struct dirent *ent;
    while ((ent = readdir(dir)) != nullptr)
    {
        std::string filename = ent->d_name;
        if (filename == "." || filename == "..")
            continue;

        std::string inputPath = inputDirPath + "/" + filename;

        struct stat st;
        if (stat(inputPath.c_str(), &st) == 0 && S_ISDIR(st.st_mode))
        {
            // Process subdirectory recursively
            std::string outputSubdirPath = outputDirPath + "/" + filename;
            if (mkdir(outputSubdirPath.c_str()) != 0 && errno != EEXIST)
            {
                std::cerr << "Error: Failed to create output subdirectory: " << outputSubdirPath << std::endl;
                continue;
            }

            ProcessDirectory(inputPath, outputSubdirPath);
        }
        else
        {
            // Process image file
            std::string extension = filename.substr(filename.find_last_of('.') + 1);
            if (extension == "jpg" || extension == "jfif" || extension == "jpeg" || extension == "png")
            {
                std::string inputImagePath = inputDirPath + "/" + filename;
                std::string outputImagePath = outputDirPath + "/" + filename.substr(0, filename.find_last_of('.')) + ".ce_image";

                std::cout << "Processing: " << inputImagePath << " -> " << outputImagePath << std::endl;

                int width, height, channels;
                unsigned char *imageData = stbi_load(inputImagePath.c_str(), &width, &height, &channels, 0);

                if (!imageData)
                {
                    std::cerr << "Error: Failed to load input image: " << inputImagePath << std::endl;
                    continue;
                }

                CustomImageFile imageFile;
                imageFile.header.width = width;
                imageFile.header.height = height;
                imageFile.header.channels = channels;
                imageFile.pixelData = imageData;

                std::ofstream outputFile(outputImagePath, std::ios::binary);
                if (!outputFile)
                {
                    std::cerr << "Error: Failed to open output file for writing: " << outputImagePath << std::endl;
                    stbi_image_free(imageData);
                    continue;
                }

                // Write the header and pixel data to the output file
                outputFile.write(reinterpret_cast<const char *>(&imageFile.header), sizeof(CustomImageHeader));
                outputFile.write(reinterpret_cast<const char *>(imageData), width * height * channels);

                outputFile.close();
                stbi_image_free(imageData);

                std::cout << "Custom image created successfully: " << outputImagePath << std::endl;
            }
        }
    }

    closedir(dir);
}
