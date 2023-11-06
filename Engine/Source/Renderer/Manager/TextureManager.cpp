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
        Load(path, {});
    }

    void TextureManager::Load(const std::string &path, TextureConfiguration config)
    {
        if (references[path])
        {
            CE_CORE_WARN("Image with path '%s' already exists, not overriding.", path.c_str());
            return;
        }

        references[path] = new TextureRef;
        references[path]->count = 0;
        references[path]->texture = new Texture();
        references[path]->texture->Load(path, config);
        CE_CORE_TRACE("TextureManager::Load: Loaded texture: '%s'.", path.c_str());
    }

    Texture *TextureManager::Get(const std::string &path)
    {
        if (!references[path])
            Load(path);

        references[path]->count++;
        CE_CORE_TRACE("Found texture: '%s', reference count is %i and generation is %i.", path.c_str(), references[path]->count, references[path]->texture->GetGeneration());

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

        if (references[path]->count == 0)
        {
            CE_CORE_WARN("TextureManager::Release: Texture '%s' is not referenced, cannot be released.");
            return;
        }

        references[path]->count--;

        CE_CORE_TRACE("TextureManager::Release: Found texture: '%s', reference count is %i.", path.c_str(), references[path]->count);
        if (references[path]->count == 0)
        {
            CE_CORE_TRACE("TextureManager::Release: Texture: '%s' is getting deleted.", path.c_str(), references[path]->count);
            delete references[path];
            references[path] = nullptr;
        }
    }

    bool TextureManager::Exist(const std::string &name)
    {
        return references[name] != nullptr;
    }

    int TextureManager::GetGlobalTextureCount()
    {
        return GTextureGenerationCount;
    }

    int TextureManager::GetTextureReferencesCount()
    {
        return references.size();
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