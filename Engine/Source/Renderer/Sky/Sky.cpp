#include "Sky.h"

#include "Core/Logger.h"
#include "Renderer/ShaderSystem.h"
#include "Renderer/Texture/CubeMapTexture.h"
#include "Renderer/Camera/CameraSystem.h"
#include "Renderer/Camera/PerspectiveCamera.h"

#include "Resources/Loaders/CubeMapConfigLoader.h"

#include <glad/glad.h>

#define CE_SKY_DEBUG_TEXTURE 0

namespace Core
{
#if CE_SKY_DEBUG_TEXTURE
    static CubeMapTexture *DEBUG = nullptr;
#endif

    static float cubeVertices[] = {
        // Front face
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,

        // Back face
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        // Left face
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,

        // Right face
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,

        // Top face
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        // Bottom face
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f};

    Sky::Sky()
    {
        color = new Color(0, 0, 0, 255);
        cubeTexture = new CubeMapTexture();

        // Setup in-world with shader
        array = new VertexArray();
        array->GenVertexBuffer(cubeVertices, sizeof(cubeVertices));
        array->GetVertexBuffer()->AddLayout(0, 0, 3);
        array->GetVertexBuffer()->Bind();

        mode = SkyMode::Color;
    }

    Sky::~Sky()
    {
        delete color;
        delete cubeTexture;
        delete array;

        for (auto a : shaderData)
            a->ClearDataBasedOnCurrentType();

        shaderData.clear();
    }

    void Sky::Render()
    {
        // WIP: Shader
        CE_PROFILE_FUNCTION();

        array->GetVertexBuffer()->Bind();
        PerspectiveCamera *camera = CameraSystem::GetActive();

        switch (mode)
        {
        case SkyMode::CubeMap:

        {
            auto shd = ShaderSystem::Get("EngineResources/Shaders/SkyBox");

            if (camera && shd && shd->IsValid())
            {
                shd->Use();
                shd->Mat4(camera->GetProjection(), "uProjection");
                shd->Mat4(camera->GetViewMatrix(), "uView");
                shd->Mat4(Matrix4::Translate(camera->GetPosition()), "uModel");

                cubeTexture->Use();
                shd->Vec4(1, 1, 1, 1, "uTint");

                shd->Int(cubeTexture->GetGeneration(), "uSkybox");
            }
            else
            {
                CE_CORE_WARN("Sky::Render: Camera is not valid, might not exist.");
            }
        }
        break;

        case SkyMode::Shader:
        {
            if (shaderName.empty())
                break;

            Shader *shd = ShaderSystem::Get(shaderName);

            if (!shd)
            {
                ShaderSystem::Load(shaderName);
                shd = ShaderSystem::Get(shaderName);

                if (!shd)
                {
                    CE_CORE_ERROR("Sky::Render: Shader '%s' is invalid.", shaderName.c_str());
                    return;
                }
            }

            shd->Use();

            // Wip Shader data
            for (auto data : shaderData)
            {
                switch (data->Type)
                {

                case Data::DataVec2:
                {
                    shd->Vec2((Vector2 *)data->Data, data->Name.c_str());
                }

                case Data::DataVec3:
                {
                    shd->Vec3((Vector3 *)data->Data, data->Name.c_str());
                }
                break;
                case Data::DataVec4:
                {
                    shd->Vec4((Vector4 *)data->Data, data->Name.c_str());
                }
                break;
                case Data::DataColor:
                {
                    Color *c = (Color *)data->Data;
                    shd->Vec4(c->r / 255, c->g / 255, c->b / 255, c->a / 255, data->Name.c_str());
                }
                break;

                case Data::DataNone:
                default:
                    CE_CORE_WARN("Sky::Render: Unknown ShaderData.Type.");
                    break;
                }
            }

#if CE_SKY_DEBUG_TEXTURE
            if (!DEBUG)
            {
                DEBUG = new CubeMapTexture();
                DEBUG->Load("Assets/CubeMaps/Nicer.ce_cubemap");
            }
            DEBUG->Use();
            shd->Int(DEBUG->GetGeneration(), "uID");
#endif

            shd->Mat4(camera->GetProjection(), "uProjection");
            shd->Mat4(camera->GetViewMatrix(), "uView");
            shd->Mat4(Matrix4::Translate(camera->GetPosition()), "uModel");
        }
        break;

        case SkyMode::Color:
            // NOTE: Color handled by the renderer at the beginning of the frame.
            break;
        }

        array->GetVertexBuffer()->Bind();
        array->GetVertexBuffer()->Draw();
    }

    void Sky::SetMode(SkyMode _mode)
    {
        mode = _mode;
    }

    void Sky::CreateCubeTexture(const std::string &filepath)
    {
        if (cubeTexture)
            delete cubeTexture;

        cubeTexture = new CubeMapTexture();
        cubeTextureConfigPath = filepath;
        cubeTexture->Load(CubeMapConfigLoader::GetConfigFromFile(filepath));
    }

    void Sky::SetShaderName(const std::string &name)
    {
        // NOTE: Delete the old one if exists.
        if (!shaderName.empty())
        {
            ShaderSystem::DestroyShader(shaderName);
        }

        shaderName = name;
    }

    void Sky::AddShaderData(CeU32 dataSize, void *Data, Data::DataType dataType, const std::string &name)
    {
        shaderData.push_back(new Data::Set(dataSize, Data, dataType, name));
    }
    void Sky::RemoveSkyShaderDataByName(const std::string &name)
    {
        // Iterator to traverse the vector
        auto it = shaderData.begin();
        while (it != shaderData.end())
        {
            if ((*it)->Name == name)
            {
                (*it)->ClearDataBasedOnCurrentType();
                delete *it;
                it = shaderData.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    void Sky::From(Sky *other)
    {
        SetMode(other->GetMode());
        this->color->Set(other->GetColor());

        if (other->GetMode() == SkyMode::CubeMap)
            CreateCubeTexture(other->GetCubeTexturePath());
        else if (other->GetMode() == SkyMode::Shader)
            SetShaderName(other->GetShaderName());

        //? Copy over the shader data
        for (auto o : other->GetSkyShaderData())
        {
            Data::Set *newSet = new Data::Set(o);
            shaderData.push_back(newSet);
        }
    }
}