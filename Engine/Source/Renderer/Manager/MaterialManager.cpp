#include "MaterialManager.h"

#include "Core/Logger.h"
#include "Resources/Loaders/MaterialLoader.h"

#include <unordered_map>

namespace Core
{
    MaterialManager::MaterialManager()
    {
    }

    MaterialManager::~MaterialManager()
    {
    }

    struct MaterialRef
    {
        int count = 0;
        Material *material;

        void Destroy()
        {
            delete material;
        };

        ~MaterialRef(){};
    };

    static std::unordered_map<std::string, MaterialRef *> references;
    static Material *defaultMaterial;

    void MaterialManager::Init()
    {
        defaultMaterial = new Material();
        defaultMaterial->GetColorTexture()->MarkAsDefault(true);
    }

    void MaterialManager::Shutdown()
    {
        for (auto it = references.begin(); it != references.end(); it++)
        {
            auto ref = it->second;
            ref->count = 0;
            delete ref;
        }

        delete defaultMaterial;
        defaultMaterial = nullptr;

        references.clear();
    }

    void MaterialManager::Load(MaterialConfiguration config)
    {
        if (references[config.name.c_str()])
        {
            CE_WARN("Material with name '%s' already exists, not overriding.", config.name.c_str());
            return;
        }

        references[config.name.c_str()] = new MaterialRef;
        references[config.name.c_str()]->count = 0;
        references[config.name.c_str()]->material = new Material(config);
        CE_INFO("Loaded material: '%s', reference count is 0.", config.name.c_str());
    }

    void MaterialManager::Load(const std::string &name)
    {
        MaterialConfiguration config = MaterialLoader::GetConfigFromFile(name);
        Load(config);
    }

    Material *MaterialManager::Get(const std::string &name)
    {
        if (!references[name])
        {
            Load(name);
        }

        references[name]->count++;
        CE_INFO("Found material: '%s', reference count is %i.", name.c_str(), references[name]->count);

        return references[name]->material;
    }

    Material *MaterialManager::GetDefault()
    {
        return defaultMaterial;
    }

    void MaterialManager::Release(const std::string &name)
    {
        if (!references[name])
            return;

        references[name]->count--;
        CE_INFO("Found material: '%s', reference count is %i.", name.c_str(), references[name]->count);

        if (references[name]->count == 0)
        {
            CE_INFO("Material: '%s' with reference count of %i is getting deleted.", name.c_str(), references[name]->count);
            references[name]->Destroy();
            delete references[name];
        }
    }
}