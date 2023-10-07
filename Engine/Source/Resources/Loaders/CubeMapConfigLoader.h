#pragma once

#include "Base.h"
#include "Renderer/Texture/CubeMapTexture.h"

namespace Core
{
    
    
    class CE_API CubeMapConfigLoader
    {
    public:
        CubeMapConfigLoader();
        ~CubeMapConfigLoader();

        static CubeMapConfiguration GetConfigFromFile(const std::string& path);
    };
}