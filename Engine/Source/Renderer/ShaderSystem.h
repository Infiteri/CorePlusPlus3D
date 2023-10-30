#pragma once

#include "Base.h"
#include "Shader.h"

#include <string>

namespace Core
{
    class CE_API ShaderSystem
    {
    public:
        ShaderSystem(){};
        ~ShaderSystem(){};

        static void Init();
        static void Shutdown();

        static Shader *Load(const std::string &name);
        static Shader *Get(const std::string &name);
        static bool UseShaderIfExists(const std::string &name);
        static void DestroyShader(const std::string &name);
    };
}