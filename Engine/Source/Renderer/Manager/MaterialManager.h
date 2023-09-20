#pragma once

#include "Base.h"

#include "Renderer/Material.h"

namespace Core
{
    class CE_API MaterialManager
    {
    public:
        MaterialManager();
        ~MaterialManager();

        static void Init();
        static void Shutdown();

        /// @brief Loads a material from the config passed in. The material's name is the named passed in.
        /// @param config A config object.
        static void Load(MaterialConfiguration config);

        /// @brief This will load from file. The material's name is the value of the "name" key. ("name = GameMaterial", the material name is GameMaterial)
        /// @param name the file name to use.
        static void Load(const std::string &name);

        /// @brief Returns a material under the passed name.
        /// @param name The name of the material to search for.
        /// @return A pointer to a Material or nullptr if non-existent.
        static Material *Get(const std::string &name);

        /// @brief Returns the default material
        /// @return A pointer to a Material or nullptr if not created.
        static Material *GetDefault();

        /// @brief Releases a material if it's no longer used.
        /// @param name The name of material.
        static void Release(const std::string &name);
    };
}