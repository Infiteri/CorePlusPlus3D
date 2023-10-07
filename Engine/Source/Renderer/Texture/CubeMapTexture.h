#pragma once

#include "Base.h"
#include "TextureBase.h"

#include <string>
#include <vector>

namespace Core
{
    struct CubeMapConfiguration
    {
        std::string left;
        std::string right;
        std::string top;
        std::string bottom;
        std::string front;
        std::string back;
        TextureConfiguration textureConfiguration;
    };

    class CE_API CubeMapTexture : public TextureBase
    {
    private:
        std::vector<std::string> filepaths;
        std::vector<Image *> images;

        void ReGenImages();

    public:
        CubeMapTexture();
        ~CubeMapTexture();

        /// @brief If its already created, call destroy before loading again
        virtual void Load() override;

        /// @brief If its already created, call destroy before loading again
        void Load(std::vector<std::string> _filepaths);

        /// @brief If its already created, call destroy before loading again
        void Load(std::vector<std::string> _filepaths, TextureConfiguration config);

        void Load(CubeMapConfiguration config);
        
        void Load(const std::string &filepath);

        void Bind();
        void Unbind();
        void Destroy();
        void Use();

        std::vector<std::string> GetFilePaths();
    };
}