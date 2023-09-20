#pragma once

#include "Renderer/Material.h"

namespace Core
{
    class MaterialLoader
    {
    public:
        MaterialLoader(){};
        ~MaterialLoader(){};

        static MaterialConfiguration GetConfigFromFile(const std::string& path);
    };
}