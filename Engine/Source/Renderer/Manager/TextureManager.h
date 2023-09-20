#pragma once

#include "Base.h"
#include "Renderer/Texture/Texture.h"

namespace Core
{
    class CE_API TextureManager
    {
    public:
        TextureManager(){};
        ~TextureManager(){};

        static void Init();
        static void Shutdown();
        static void Load(const std::string &path);
        static Texture *Get(const std::string &path);
        static Texture *GetDefault();
        static void Release(const std::string &path);
    };
}