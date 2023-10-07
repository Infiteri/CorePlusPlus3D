#include "CubeMapTexture.h"

#include "Core/Logger.h"
#include "Core/Timer.h"

#include "Renderer/Manager/TextureManager.h"

#include "Resources/Loaders/CubeMapConfigLoader.h"

#include <glad/glad.h>

namespace Core
{
    CubeMapTexture::CubeMapTexture()
    {
        image = nullptr;
        generation = TextureManager::GetGlobalTextureCount();
        TextureManager::IncrementGlobalTextureCount();
    }

    CubeMapTexture::~CubeMapTexture()
    {
        Destroy();
    }

    void CubeMapTexture::Load()
    {
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

        glGenTextures(1, &id);
        Bind();

        for (int i = 0; i < 6; i++)
        {
            Timer timer = Timer("Load Image");

            Image *img = new Image(_filepaths[i]);
            images.push_back(img);

            timer.Stop();
            CE_TRACE("%s: %f.", _filepaths[i].c_str(), timer.GetTimeDiff());

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, img->GetWidth(), img->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, img->GetData());
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        //? state
        filepaths = _filepaths;
        generation = TextureManager::GetGlobalTextureCount();
        TextureManager::IncrementGlobalTextureCount();

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

        glGenTextures(1, &id);
        Bind();

        for (int i = 0; i < 6; i++)
        {
            Timer timer = Timer("Load Image");

            Image *img = new Image(_filepaths[i]);
            images.push_back(img);

            timer.Stop();
            CE_TRACE("%s: %f.", _filepaths[i].c_str(), timer.GetTimeDiff());

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, img->GetWidth(), img->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, img->GetData());
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, TextureFilterToOpenGL(config.min));
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, TextureFilterToOpenGL(config.mag));
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        //? state
        filepaths = _filepaths;
        generation = TextureManager::GetGlobalTextureCount();
        TextureManager::IncrementGlobalTextureCount();

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
            delete img;

        glDeleteTextures(1, &id);

        generation = 0;
        TextureManager::DecrementGlobalTextureCount();

        images.clear();
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