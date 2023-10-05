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

    Texture::Texture()
    {
        generation = TextureManager::GetGlobalTextureCount();
        TextureManager::IncrementGlobalTextureCount();
    }

    Texture::~Texture()
    {
        Destroy();
    }

    void Texture::Load()
    {
        image = nullptr;

        glGenTextures(1, &id);
        Bind();

        CeU8 data[4] = {255, 255, 255, 255};
        TextureLoadUtils(1, 1, data, GL_RGBA, {});

        Unbind();
    }

    void Texture::Load(const std::string &_filepath)
    {
        image = new Image(_filepath);

        CE_INFO("%s, %i", _filepath.c_str(), isDefault);

        glGenTextures(1, &id);
        Bind();
        TextureLoadUtils(image->GetWidth(), image->GetHeight(), image->GetData(), ChannelToGL(image->GetChannels()), {});
        Unbind();
    }

    void Texture::Load(const std::string &_filepath, TextureConfiguration config)
    {
        image = new Image(_filepath);

        CE_INFO("%s, %i", _filepath.c_str(), isDefault);

        glGenTextures(1, &id);
        Bind();
        TextureLoadUtils(image->GetWidth(), image->GetHeight(), image->GetData(), ChannelToGL(image->GetChannels()), config);
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
        TextureManager::DecrementGlobalTextureCount();
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

    bool Texture::IsMarkedAsDefault()
    {
        return isDefault;
    }
}