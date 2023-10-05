#pragma once

#include "Base.h"
#include "TextureBase.h"

#include <string>
#include <vector>

namespace Core
{
    class CE_API CubeMapTexture : public TextureBase
    {
    private:
        std::vector<std::string> filepaths;
        std::vector<Image *> images;

    public:
        CubeMapTexture();
        ~CubeMapTexture();

        /// @brief If its already created, call destroy before loading again
        virtual void Load() override;

        /// @brief If its already created, call destroy before loading again
        void Load(std::vector<std::string> _filepaths);

        /// @brief If its already created, call destroy before loading again
        void Load(std::vector<std::string> _filepaths, TextureConfiguration config);

        void Bind();
        void Unbind();
        void Destroy();
        void Use();

        std::vector<std::string> GetFilePaths();
    };
}