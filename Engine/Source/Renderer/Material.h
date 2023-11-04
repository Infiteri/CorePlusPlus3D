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
        /// @brief The material name which is now a path, used everywhere related to materials.
        std::string name;

        /// @brief Color of the material to use.
        Color color;

        /// @brief Leave empty for default
        std::string colorTextureName;

        
        /// @brief Leave empty for default
        std::string normalTextureName;

        float shininess;
    };

    class CE_API Material
    {
    private:
        Color color;
        std::string name;
        Texture *texture;
        Texture *normalTexture;
        float shininess;

    public:
        Material();
        Material(MaterialConfiguration config);
        ~Material();

        std::string GetName();
        std::string GetColorTexturePath();

        void From(Material *other);

        void SetName(const std::string &name);

        /// @brief Returns the color of the materials, could use "->Set" on the return value to set the color's values.
        /// @return A pointer to the material color.
        Color *GetColor();

        /// @brief Returns a pointer to the color texture, could be empty, default or custom. (Based on the colorTextureName)
        /// @return A pointer to a Texture object.
        Texture *GetColorTexture();
        void SetColorTexture(const std::string &name);

        Texture *GetNormalTexture();
        void SetNormalTexture(const std::string &name);

        /// @brief Do not call yourself.
        void Use();

        /// @brief Called in the destructor, could have uses of its own.
        void Destroy();

        float GetShininess();
        void SetShininess(float a);
    };
}