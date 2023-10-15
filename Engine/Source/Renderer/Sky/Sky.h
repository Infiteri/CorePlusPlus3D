#pragma once

#include "Base.h"
#include "Renderer/Color.h"
#include "Renderer/Buffer/VertexArray.h"

#include "Renderer/Texture/CubeMapTexture.h"

namespace Core
{
    /// @brief Enumerations that represent the types of sky modes.
    enum class SkyMode
    {
        /// @brief Basic colors.
        Color,

        /// @brief Cube Map.
        CubeMap,
    };

    class CE_API Sky
    {
    private:
        Color *color = nullptr;
        VertexArray *array = nullptr;
        CubeMapTexture *cubeTexture = nullptr;
        std::string cubeTextureConfigPath = "";
        SkyMode mode;

    public:
        Sky();
        ~Sky();

        void Render();

        void SetMode(SkyMode _mode);

        /// @brief Returns the current Sky Mode, used by the editor but might have other uses.
        /// @return A element from the enumeration SkyMode.
        inline SkyMode GetMode() const { return mode; };

        /// @brief Color makes sense only when sky mode is set to color, the sky will use a clear color instead of a cube map or shader.
        /// @return Pointer to Color instance / object.
        inline Color *GetColor() const { return color; };

        void CreateCubeTexture(const std::string &filepath);
        inline CubeMapTexture *GetCubeTexture() { return cubeTexture; };
        inline std::string GetCubeTexturePath() { return cubeTextureConfigPath; };

        void From(Sky *other);
    };
}