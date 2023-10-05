#include "TextureManager.h"

#include "Core/Logger.h"

#include "unordered_map"
#include <string>

namespace Core
{
    static Texture *defaultTexture;
    static CeU32 GTextureGenerationCount = 0;

    struct TextureRef
    {
        int count = 0;
        Texture *texture;

        ~TextureRef()
        {
            delete texture;
        };
    };
    static std::unordered_map<std::string, TextureRef *> references;

    void TextureManager::Init()
    {
        defaultTexture = new Texture();
        defaultTexture->Load();
    }

    void TextureManager::Shutdown()
    {
        for (auto it = references.begin(); it != references.end(); it++)
        {
            auto ref = it->second;

            if (ref != nullptr)
            {
                ref->count = 0;
                delete ref;
            }
        }

        delete defaultTexture;
        references.clear();
    }

    void TextureManager::Load(const std::string &path)
    {
        if (references[path])
        {
            CE_WARN("Image with path '%s' already exists, not overriding.", path.c_str());
            return;
        }

        references[path] = new TextureRef;
        references[path]->count = 0;
        references[path]->texture = new Texture();
        references[path]->texture->Load(path);
        CE_INFO("Loaded texture: '%s', reference count is 0.", path.c_str());
    }

    Texture *TextureManager::Get(const std::string &path)
    {
        if (!references[path])
        {
            Load(path);
        }

        references[path]->count++;
        CE_INFO("Found texture: '%s', reference count is %i.", path.c_str(), references[path]->count);

        return references[path]->texture;
    }

    Texture *TextureManager::GetDefault()
    {
        return defaultTexture;
    }

    Texture *TextureManager::GetNewEmpty()
    {
        Texture *tex = new Texture();
        tex->Load();
        return tex;
    }

    void TextureManager::Release(const std::string &path)
    {
        if (!references[path])
            return;

        references[path]->count--;
        CE_INFO("Found texture: '%s', reference count is %i.", path.c_str(), references[path]->count);
        if (references[path]->count == 0)
        {
            CE_INFO("Texture: '%s' with reference count of %i is getting deleted.", path.c_str(), references[path]->count);
            delete references[path];
            references[path] = nullptr;
        }
    }

    int TextureManager::GetGlobalTextureCount()
    {
        return GTextureGenerationCount;
    }

    void TextureManager::IncrementGlobalTextureCount()
    {
        GTextureGenerationCount++;
    }

    void TextureManager::DecrementGlobalTextureCount()
    {
        GTextureGenerationCount--;
    }
}