#include "TextureBase.h"

#include <glad/glad.h>

namespace Core
{
    TextureBase::TextureBase()
    {
        id = 0;
        generation = 0;
        image = nullptr;
    }

    TextureBase::~TextureBase()
    {
    }

    void TextureBase::Load()
    {
    }

    void TextureBase::Bind()
    {
    }

    void TextureBase::Unbind()
    {
    }

    void TextureBase::Destroy()
    {
    }

    void TextureBase::Use()
    {
    }

    bool TextureBase::HasImage()
    {
        return image != nullptr;
    }

    std::string TextureBase::GetImagePath()
    {
        if (!HasImage())
            return "";

        return image->GetPath().c_str();
    }

    CeU32 TextureBase::GetID()
    {
        return id;
    }

    CeU32 TextureBase::GetGeneration()
    {
        return generation;
    }

    CeU32 TextureFilterToOpenGL(TextureFilter filter)
    {
        if (filter == TextureFilter::Nearest)
            return GL_NEAREST;
        else if (filter == TextureFilter::Linear)
            return GL_LINEAR;
        else if (filter == TextureFilter::LinearMipMap)
            return GL_LINEAR_MIPMAP_LINEAR;
        else if (filter == TextureFilter::NearestMipMap)
            return GL_NEAREST_MIPMAP_NEAREST;

        return GL_NEAREST;
    }

    void TextureLoadUtils(int w, int h, CeU8 *data, CeU32 channel, TextureConfiguration config)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, channel, w, h, 0, channel, GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, TextureFilterToOpenGL(config.min));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, TextureFilterToOpenGL(config.mag));

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    };
}