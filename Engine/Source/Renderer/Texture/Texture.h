#pragma once

#include "Base.h"
#include "Resources/Image.h"

#include <string>

namespace Core
{
    enum class TextureFilter
    {
        Nearest,
        Linear,
        NearestMipMap,
        LinearMipMap
    };

    struct TextureConfiguration
    {
        TextureFilter min = TextureFilter::Linear;
        TextureFilter mag = TextureFilter::Linear;
    };

    /// @brief Simple image texture.
    /// Image: The actual image data
    /// Texture: Id, Generation and uses the image
    class CE_API Texture
    {
    private:
        CeU32 id;
        CeU32 generation;
        Image *image;
        bool isDefault;

    public:
        Texture();
        ~Texture();

        /// @brief If its already created, call destroy before loading again
        void Load();

        /// @brief If its already created, call destroy before loading again
        void Load(const std::string &_filepath);

        /// @brief If its already created, call destroy before loading again
        void Load(const std::string &_filepath, TextureConfiguration config);

        void Bind();
        void Unbind();
        void Destroy();
        void Use();

        /// @brief Do not call unless you're the TextureManager. If this is marked as default and its not it will not be cleared in the material.
        void MarkAsDefault(bool flag);

        /// @brief If Load is called without any args than it has no image
        /// @return Boolean
        bool HasImage();

        bool IsMarkedAsDefault();

        std::string GetImagePath();

        CeU32 GetID();
        CeU32 GetGeneration();
    };
}