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
        static void Load(const std::string &path, TextureConfiguration config);
        static Texture *Get(const std::string &path);
        static Texture *GetDefault();
        static Texture *GetNewEmpty();
        static void Release(const std::string &path);

        static bool Exist(const std::string& name);

        static int GetGlobalTextureCount();
        static void IncrementGlobalTextureCount();
        static void DecrementGlobalTextureCount();
    };
}