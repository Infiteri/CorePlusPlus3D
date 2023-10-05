#pragma once

#include "Base.h"
#include "Resources/Image.h"

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

    CeU32 TextureFilterToOpenGL(TextureFilter filter);
    void TextureLoadUtils(int w, int h, CeU8 *data, CeU32 channel, TextureConfiguration config);

    class TextureBase
    {
    protected:
        CeU32 id;
        CeU32 generation;
        Image *image;

    public:
        TextureBase();
        virtual ~TextureBase();

        /// @brief If its already created, call destroy before loading again
        virtual void Load();

        virtual void Bind();
        virtual void Unbind();
        virtual void Destroy();
        virtual void Use();

        /// @brief If Load is called without any args than it has no image
        /// @return Boolean
        bool HasImage();
        std::string GetImagePath();
        CeU32 GetID();
        CeU32 GetGeneration();
    };
}
