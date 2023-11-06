#include "ShaderSystem.h"

#include "Core/Logger.h"

#include <unordered_map>

namespace Core
{
    static std::unordered_map<std::string, Shader *> shaders;

    void ShaderSystem::Init()
    {
    }

    void ShaderSystem::Shutdown()
    {
        for (auto it = shaders.begin(); it != shaders.end(); it++)
        {
            Shader *shd = it->second;

            delete shd;
        }

        shaders.clear();
    }

    Shader *ShaderSystem::Load(const std::string &name)
    {
        Shader *shader = new Shader(name + ".vs.glsl", name + ".fs.glsl");

        if (!shader->IsValid())
        {
            CE_CORE_ERROR("Unable to load shader: %s, not valid.", name.c_str());
            return nullptr;
        }

        shaders[name] = shader;

        return shader;
    }

    Shader *ShaderSystem::Get(const std::string &name)
    {
        return shaders[name] ? shaders[name] : nullptr;
    }

    bool ShaderSystem::UseShaderIfExists(const std::string &name)
    {
        if (shaders[name] != nullptr)
        {
            shaders[name]->Use();
            return true;
        }

        return false;
    }

    void ShaderSystem::DestroyShader(const std::string &name)
    {
        if (shaders[name])
        {
            delete shaders[name];
            shaders.erase(name);
        }
        else
        {
            CE_CORE_WARN("ShaderSystem::Destroy: No shader at name '%s'.", name.c_str());
        }
    }
}