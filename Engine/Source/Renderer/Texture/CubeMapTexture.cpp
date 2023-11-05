#include "CubeMapTexture.h"

#include "Core/Logger.h"
#include "Core/Timer.h"

#include "Renderer/Manager/TextureManager.h"

#include "Resources/Loaders/CubeMapConfigLoader.h"

#include <glad/glad.h>

namespace Core
{
    void CubeMapTexture::ReGenImages()
    {
        for (int i = 0; i < 6; i++)
        {
            Image *img = new Image(filepaths[i]);
            images.push_back(img);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB8, img->GetWidth(), img->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, img->GetData());
        }
    }

    void CubeMapTexture::FreeImagesData()
    {
        for (int i = 0; i < 6; i++)
        {
            Image *img = images[i];
            img->FreeData();
            delete img;
            img = nullptr;
        }

        images.clear();
    }

    CubeMapTexture::CubeMapTexture()
    {
        id = 0;
        generation = 0;
        image = nullptr;
    }

    CubeMapTexture::~CubeMapTexture()
    {
        Destroy();
    }

    void CubeMapTexture::Load()
    {
        if (generation == 0)
        {
            generation = TextureManager::GetGlobalTextureCount();
            TextureManager::IncrementGlobalTextureCount();
        }

        glGenTextures(1, &id);
        Bind();

        CeU8 data[3] = {0, 0, 0};

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        Unbind();
    }

    void CubeMapTexture::Load(std::vector<std::string> _filepaths)
    {
        if (_filepaths.size() > 6)
            CE_WARN("The amount of paths cannot be over 6.");

        if (_filepaths.size() < 6)
        {
            CE_ERROR("The amount of paths cannot be less then 6.");
            return;
        }

        if (generation == 0)
        {
            generation = TextureManager::GetGlobalTextureCount();
            TextureManager::IncrementGlobalTextureCount();
        }

        //? state
        filepaths = _filepaths;
        glGenTextures(1, &id);
        Bind();

        ReGenImages();

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        FreeImagesData();

        Unbind();
    }

    void CubeMapTexture::Load(std::vector<std::string> _filepaths, TextureConfiguration config)
    {
        if (_filepaths.size() > 6)
            CE_WARN("The amount of paths cannot be over 6.");

        if (_filepaths.size() < 6)
        {
            CE_ERROR("The amount of paths cannot be less then 6.");
            return;
        }

        filepaths = _filepaths;
        if (generation == 0)
        {
            generation = TextureManager::GetGlobalTextureCount();
            TextureManager::IncrementGlobalTextureCount();
        }

        glGenTextures(1, &id);
        Bind();

        ReGenImages();

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, TextureFilterToOpenGL(config.min));
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, TextureFilterToOpenGL(config.mag));
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        FreeImagesData();

        Unbind();
    }

    void CubeMapTexture::Load(CubeMapConfiguration config)
    {
        Load({config.right, config.left, config.top, config.bottom, config.front, config.back}, config.textureConfiguration);
    }

    void CubeMapTexture::Load(const std::string &filepath)
    {
        CubeMapConfiguration c = CubeMapConfigLoader::GetConfigFromFile(filepath);
        Load(c);
    }

    void CubeMapTexture::Bind()
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    }

    void CubeMapTexture::Unbind()
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    }

    void CubeMapTexture::Destroy()
    {
        for (Image *img : images)
        {
            if (img != nullptr)
                delete img;
        }

        glDeleteTextures(1, &id);

        if (generation != 0)
        {
            generation = 0;
            TextureManager::DecrementGlobalTextureCount();
        }
    }

    void CubeMapTexture::Use()
    {
        glActiveTexture(GL_TEXTURE0 + generation);
        Bind();
    }

    std::vector<std::string> CubeMapTexture::GetFilePaths()
    {
        return filepaths;
    }
}