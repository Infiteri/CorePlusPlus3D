#include "Texture.h"

#include "Core/Logger.h"
#include "Renderer/Manager/TextureManager.h"

#include <glad/glad.h>

namespace Core
{
    static GLenum ChannelToGL(int c)
    {
        return c == 3 ? GL_RGB : GL_RGBA;
    };

    static CeU8 defaultRGBAData[4] = {255, 255, 255, 255};

    Texture::Texture()
    {
        id = 0;
        generation = 0;
        image = nullptr;
    }

    Texture::~Texture()
    {
        Destroy();
    }

    void Texture::Load()
    {
        if (generation == 0)
        {
            CE_DEBUG("Generation increased for new empty texture.");

            generation = TextureManager::GetGlobalTextureCount();
            TextureManager::IncrementGlobalTextureCount();
        }

        if (image)
            delete image;

        image = nullptr;

        glGenTextures(1, &id);
        Bind();

        TextureLoadUtils(1, 1, defaultRGBAData, GL_RGBA, {});

        Unbind();
    }

    void Texture::Load(const std::string &_filepath)
    {
        if (generation == 0)
        {
            CE_DEBUG("Generation increased for '%s'", _filepath.c_str());
            generation = TextureManager::GetGlobalTextureCount();
            TextureManager::IncrementGlobalTextureCount();
        }

        image = new Image(_filepath);

        glGenTextures(1, &id);
        Bind();
        TextureLoadUtils(image->GetWidth(), image->GetHeight(), image->GetData(), ChannelToGL(image->GetChannels()), {});
        Unbind();
        image->FreeData();
    }

    void Texture::Load(const std::string &_filepath, TextureConfiguration config)
    {
        image = new Image(_filepath);

        if (generation == 0)
        {
            CE_DEBUG("Generation increased for '%s' which is created with a configuration.", _filepath.c_str());
            generation = TextureManager::GetGlobalTextureCount();
            TextureManager::IncrementGlobalTextureCount();
        }

        glGenTextures(1, &id);
        Bind();
        TextureLoadUtils(image->GetWidth(), image->GetHeight(), image->GetData(), ChannelToGL(image->GetChannels()), config);
        Unbind();
        image->FreeData();
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
        TextureManager::DecrementGlobalTextureCount();
    }

    void Texture::Use()
    {
        glActiveTexture(GL_TEXTURE0 + generation);
        Bind();
    }

    void Texture::Load(CeU8 *data, int channel, CeU32 w, CeU32 h)
    {
        glGenTextures(1, &id);
        Bind();
        TextureLoadUtils(w, h, data, ChannelToGL(channel), {});
        Unbind();
    }

    int Texture::GetWidth()
    {
        if (!image || image == nullptr)
            return 0;

        return image->GetWidth();
    }

    int Texture::GetHeight()
    {
        if (!image || image == nullptr)
            return 0;

        return image->GetHeight();
    }

    void Texture::MarkAsDefault(bool flag)
    {
        isDefault = flag;
    }

    bool Texture::IsMarkedAsDefault()
    {
        return isDefault;
    }
}