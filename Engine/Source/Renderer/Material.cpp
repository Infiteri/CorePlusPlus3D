#include "Material.h"

#include "Core/Logger.h"
#include "Shader.h"

#include "Renderer.h"
#include "Manager/TextureManager.h"

namespace Core
{
    Material::Material()
    {
        name = "DefaultMaterial";
        color.Set(255, 255, 255, 255);
        texture = TextureManager::GetNewEmpty();
    }

    Material::Material(MaterialConfiguration config)
    {
        name = config.name;
        color.Set(config.color.r, config.color.g, config.color.b, config.color.a);

        if (config.colorTextureName.empty())
            texture = TextureManager::GetNewEmpty();
        else
            texture = TextureManager::Get(config.colorTextureName);
    }

    Material::~Material()
    {
        Destroy();
    }

    std::string Material::GetName()
    {
        return name;
    }

    void Material::From(Material *other)
    {
        SetName(other->GetName());
        color.Set(other->GetColor());
        if (other->GetColorTexture()->HasImage())
            SetColorTexture(other->GetColorTexture()->GetImagePath());
    }

    void Material::SetName(const std::string &name)
    {
        this->name = name;
    }

    Color *Material::GetColor()
    {
        return &color;
    }

    Texture *Material::GetColorTexture()
    {
        return texture;
    }

    void Material::SetColorTexture(const std::string &name)
    {
        if (name.empty())
            return;

        if (texture && !texture->IsMarkedAsDefault() && texture->HasImage())
        {
            TextureManager::Release(texture->GetImagePath());
            texture = nullptr;
        }
        else if (texture && !texture->IsMarkedAsDefault())
        {
            delete texture;
            texture = nullptr;
        }
        texture = TextureManager::Get(name);
    }

    void Material::Use()
    {
        Shader *shader = Renderer::GetObjectShader();

        if (!shader || !texture || texture == nullptr)
            return;

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
