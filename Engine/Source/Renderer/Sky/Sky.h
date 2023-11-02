#pragma once

#include "Base.h"
#include "Core/Memory/CeMemory.h"
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

    enum class SkyShaderDataType
    {
        None = 0,
        Vec2,
        Vec3,
        Vec4,
        Color
    };

    struct SkyShaderData
    {
        std::string Name;
        SkyShaderDataType type;
        void *Data;
        bool shouldClear = false;

        SkyShaderData(CeU32 dataSize, void *_Data, SkyShaderDataType dataType, const std::string &_name)
        {
            shouldClear = true;
            type = dataType;
            Name = _name;

            switch (type)
            {
            case SkyShaderDataType::Vec2:
            {
                Vector2 *other = static_cast<Vector2 *>(_Data);
                Data = new Vector2(other->x, other->y);
                break;
            }

            case SkyShaderDataType::Vec3:
            {
                Vector3 *other = static_cast<Vector3 *>(_Data);
                Data = new Vector3(other->x, other->y, other->z);
                break;
            }

            case SkyShaderDataType::Vec4:
            {
                Vector4 *other = static_cast<Vector4 *>(_Data);
                Data = new Vector4(other->x, other->y, other->z, other->w);
                break;
            }

            case SkyShaderDataType::Color:
            {
                Color *other = static_cast<Color *>(_Data);
                Data = new Color(other->r, other->g, other->b, other->a);
                break;
            }

            case SkyShaderDataType::None:
            default:
                // Optionally handle or log an error for an unsupported type.
                break;
            }
        }

        CE_API void ClearDataBasedOnCurrentType();

        /// @brief ClearDataBasedOnCurrentType IS NOT CALLED! CALL AT YOUR OWN
        CE_API void SetupDefaultValuesBaseOnCurrentType();
    };

    class CE_API Sky
    {
    private:
        Color *color = nullptr;
        VertexArray *array = nullptr;
        CubeMapTexture *cubeTexture = nullptr;
        std::string cubeTextureConfigPath = "";

        std::string shaderName = "";
        std::vector<SkyShaderData *> shaderData; // NOTE: BRO THIS IS CORRECT DONT WRITE OTHERWISE, FIX THE CODE

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
        void AddShaderData(CeU32 dataSize, void *Data, SkyShaderDataType dataType, const std::string &name);

        inline std::vector<SkyShaderData *> GetSkyShaderData() { return shaderData; };
        void RemoveSkyShaderDataByName(const std::string &name);

        void From(Sky *other);
    };
}