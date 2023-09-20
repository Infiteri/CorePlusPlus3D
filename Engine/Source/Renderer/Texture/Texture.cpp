#include "Texture.h"

#include <glad/glad.h>

namespace Core
{
    static CeU32 globalGens = 0;

    static GLenum FilterToGL(TextureFilter filter)
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
    };

    static void LoadUtil(int w, int h, CeU8 *data, GLenum channel, TextureConfiguration config)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, channel, w, h, 0, channel, GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilterToGL(config.min));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FilterToGL(config.mag));

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    };

    static GLenum ChannelToGL(int c)
    {
        return c == 3 ? GL_RGB : GL_RGBA;
    };

    Texture::Texture()
    {
        image = nullptr;
        id = 0;
        generation = globalGens;
        globalGens++;
    }

    Texture::~Texture()
    {
        Destroy();
    }

    void Texture::Load()
    {
        glGenTextures(1, &id);
        Bind();

        CeU8 data[4] = {255, 255, 255, 255};
        LoadUtil(1, 1, data, GL_RGBA, {});

        Unbind();
    }

    void Texture::Load(const std::string &_filepath)
    {
        image = new Image(_filepath);

        glGenTextures(1, &id);
        Bind();
        LoadUtil(image->GetWidth(), image->GetHeight(), image->GetData(), ChannelToGL(image->GetChannels()), {});
        Unbind();
    }

    void Texture::Load(const std::string &_filepath, TextureConfiguration config)
    {
        image = new Image(_filepath);

        glGenTextures(1, &id);
        Bind();
        LoadUtil(image->GetWidth(), image->GetHeight(), image->GetData(), ChannelToGL(image->GetChannels()), config);
        Unbind();
    }

    void Texture::Bind()
    {
        glBindTexture(GL_TEXTURE_2D, id);
    }

    void Texture::Unbind()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture::Destroy()
    {
        if (image != nullptr)
        {
            delete image;
            image = nullptr;
        }

        glDeleteTextures(1, &id);

        generation = 0;
        globalGens--;
    }

    void Texture::Use()
    {
        glActiveTexture(GL_TEXTURE0 + generation);
        Bind();
    }

    void Texture::MarkAsDefault(bool flag)
    {
        isDefault = flag;
    }

    bool Texture::HasImage()
    {
        return image != nullptr;
    }

    bool Texture::IsMarkedAsDefault()
    {
        return isDefault;
    }

    std::string Texture::GetImagePath()
    {
        if (!image)
            return "";

        return image->GetPath();
    }

    CeU32 Texture::GetID()
    {
        return id;
    }

    CeU32 Texture::GetGeneration()
    {
        return generation;
    }
}