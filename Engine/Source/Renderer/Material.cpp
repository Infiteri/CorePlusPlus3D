#include "Material.h"

#include "Shader.h"

#include "Renderer.h"
#include "Manager/TextureManager.h"

namespace Core
{
    Material::Material()
    {
        name = "DefaultMaterial";
        color.Set(255, 255, 255, 255);
        texture = TextureManager::GetDefault();
    }

    Material::Material(MaterialConfiguration config)
    {
        color.Set(config.color.r, config.color.g, config.color.b, config.color.a);

        name = config.name;

        config.colorTextureName.empty() ? texture = TextureManager::GetDefault() : texture = TextureManager::Get(config.colorTextureName);
    }

    Material::~Material()
    {
        Destroy();
    }

    std::string Material::GetName()
    {
        return name;
    }

    Color *Material::GetColor()
    {
        return &color;
    }

    Texture *Material::GetColorTexture()
    {
        return texture;
    }

    void Material::Use()
    {
        Shader *shader = Renderer::GetObjectShader();

        shader->Vec4(color.r / 255, color.g / 255, color.b / 255, color.a / 255, "uColor");

        texture->Use();
        shader->Int(texture->GetGeneration(), "uColorTexture");
    }

    void Material::Destroy()
    {
        if (texture && !texture->IsMarkedAsDefault())
        {
            if (texture->HasImage())
                TextureManager::Release(texture->GetImagePath());
            else
            {
                // DONE: delete texture only if its not default
                delete texture;
            }

            texture = nullptr;
        }
    }
}
