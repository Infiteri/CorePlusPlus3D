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
        normalTexture = TextureManager::GetNewEmpty();
        shininess = 32;
    }

    Material::Material(MaterialConfiguration config)
    {
        name = config.name;
        color.Set(config.color.r, config.color.g, config.color.b, config.color.a);
        shininess = config.shininess;

        if (config.colorTextureName.empty())
            texture = TextureManager::GetNewEmpty();
        else
            texture = TextureManager::Get(config.colorTextureName);

        if (config.normalTextureName.empty())
            normalTexture = TextureManager::GetNewEmpty();
        else
            normalTexture = TextureManager::Get(config.normalTextureName);
    }

    Material::~Material()
    {
        Destroy();
    }

    std::string Material::GetName()
    {
        return name;
    }

    std::string Material::GetColorTexturePath()
    {
        if (!texture)
            return "";
        return texture->GetImagePath();
    }

    void Material::From(Material *other)
    {
        SetName(other->GetName());
        color.Set(other->GetColor());
        if (other->GetColorTexture()->HasImage())
            SetColorTexture(other->GetColorTexture()->GetImagePath());

        if (other->GetNormalTexture()->HasImage())
            SetNormalTexture(other->GetNormalTexture()->GetImagePath());
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
        {
            if (!texture->IsMarkedAsDefault())
            {
                delete texture;
                texture = new Texture();
                texture->Load();
            }

            return;
        }

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

    Texture *Material::GetNormalTexture()
    {
        return normalTexture;
    }

    void Material::SetNormalTexture(const std::string &name)
    {
        if (name.empty())
        {
            if (!normalTexture->IsMarkedAsDefault())
            {
                delete normalTexture;
                normalTexture = new Texture();
                normalTexture->Load();
            }

            return;
        }

        if (normalTexture && !normalTexture->IsMarkedAsDefault() && normalTexture->HasImage())
        {
            TextureManager::Release(normalTexture->GetImagePath());
            normalTexture = nullptr;
        }
        else if (normalTexture && !normalTexture->IsMarkedAsDefault())
        {
            delete normalTexture;
            normalTexture = nullptr;
        }
        normalTexture = TextureManager::Get(name);
    }

    void Material::Use()
    {
        CE_PROFILE_FUNCTION();

        Shader *shader = Renderer::GetObjectShader();

        if (!shader || !texture || texture == nullptr || !normalTexture)
            return;

        shader->Vec4(color.r / 255, color.g / 255, color.b / 255, color.a / 255, "uColor");
        texture->Use();
        shader->Int(texture->GetGeneration(), "uColorTexture");
        shader->Int(normalTexture->GetGeneration(), "uNormalTexture");
        shader->Float(shininess, "uShininess");
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

        if (normalTexture && !normalTexture->IsMarkedAsDefault())
        {
            if (normalTexture->HasImage())
                TextureManager::Release(normalTexture->GetImagePath());
            else
            {
                // DONE: delete normalTexture only if its not default
                delete normalTexture;
            }

            normalTexture = nullptr;
        }
    }

    float Material::GetShininess()
    {
        return shininess;
    }

    void Material::SetShininess(float a)
    {
        shininess = a;
    }
}
