#pragma once

#include "Base.h"
#include "Core/Memory/CeMemory.h"
#include "Core/CeData.h"
#include "Math/Vectors.h"
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

        /// @brief Shader Mode.
        Shader,
    };

        class CE_API Sky
    {
    private:
        Color *color = nullptr;
        VertexArray *array = nullptr;
        CubeMapTexture *cubeTexture = nullptr;
        std::string cubeTextureConfigPath = "";

        std::string shaderName = "";
        std::vector<Data::Set *> shaderData; // NOTE: BRO THIS IS CORRECT DONT WRITE OTHERWISE, FIX THE CODE

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

        inline std::string GetShaderName() { return shaderName; };
        void SetShaderName(const std::string &name);
        void AddShaderData(CeU32 dataSize, void *Data, Data::DataType dataType, const std::string &name);

        inline std::vector<Data::Set *> GetSkyShaderData() { return shaderData; };
        void RemoveSkyShaderDataByName(const std::string &name);

        void From(Sky *other);
    };
}