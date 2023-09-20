#pragma once

#include "Base.h"
#include "Color.h"
#include "Texture/Texture.h"

#include <string>

namespace Core
{
    /// @brief Structure that represents a materials proprieties.
    /// @param Name the materials name, materials are referenced fully by their name.
    /// @param Color the color to use, could be used for tinting the texture.
    /// @param ColorTextureName The texture to use for the color texture, if not empty, it will get a texture with this name, please keep in mind that is a texture path and not just a simple name.
    struct MaterialConfiguration
    {
        /// @brief The material name, used everywhere related to materials.
        std::string name;

        /// @brief Color of the material to use.
        Color color;

        /// @brief Leave empty for default
        std::string colorTextureName;
    };

    class CE_API Material
    {
    private:
        Color color;
        std::string name;
        Texture *texture;

    public:
        Material();
        Material(MaterialConfiguration config);
        ~Material();

        /// @brief Returns the material's name, either "DefaultMaterial" or the name passed in the 'config' param when constructing material or the value of key "name"
        /// in file if loaded from a file.
        /// @return The materials name.
        std::string GetName();

        /// @brief Returns the color of the materials, could use "->Set" on the return value to set the color's values.
        /// @return A pointer to the material color.
        Color *GetColor();

        /// @brief Returns a pointer to the color texture, could be empty, default or custom. (Based on the colorTextureName)
        /// @return A pointer to a Texture object.
        Texture *GetColorTexture();

        /// @brief Do not call yourself.
        void Use();

        /// @brief Called in the destructor, could have uses of its own.
        void Destroy();
    };
}