#include "Sky.h"

#include "Core/Logger.h"
#include "Renderer/ShaderSystem.h"
#include "Renderer/Camera/CameraSystem.h"
#include "Renderer/Camera/PerspectiveCamera.h"

#include "Resources/Loaders/CubeMapConfigLoader.h"

#include <glad/glad.h>

namespace Core
{
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

    void SkyShaderData::ClearDataBasedOnCurrentType()
    {
        if (!shouldClear)
            return;

        shouldClear = false;
        switch (type)
        {
        case SkyShaderDataType::Vec2:
        {
            Vector2 *d = (Vector2 *)Data;
            delete d;
        }
        break;

        case SkyShaderDataType::Vec3:
        {
            Vector3 *d = (Vector3 *)Data;
            delete d;
        }
        break;

        case SkyShaderDataType::Vec4:
        {
            Vector4 *d = (Vector4 *)Data;
            delete d;
        }
        break;

        case SkyShaderDataType::Color:
        {
            Color *d = (Color *)Data;
            delete d;
        }
        break;

        case SkyShaderDataType::None:
        default:
            shouldClear = true;
            break;
        }
    }

    void SkyShaderData::SetupDefaultValuesBaseOnCurrentType()
    {
        shouldClear = true;
        switch (type)
        {
        case SkyShaderDataType::Vec2:
            Data = new Vector2(0, 0);
            break;

        case SkyShaderDataType::Vec3:
            Data = new Vector3(0, 0, 0);
            break;

        case SkyShaderDataType::Vec4:
            Data = new Vector4(0, 0, 0, 0);
            break;

        case SkyShaderDataType::Color:
            Data = new Color(255, 255, 255, 255);
            break;

        case SkyShaderDataType::None:
        default:
            break;
        }
    }

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

        array->GetVertexBuffer()->Bind();
        auto shd = ShaderSystem::Get("EngineResources/Shaders/SkyBox");
        PerspectiveCamera *camera = CameraSystem::GetActive();

        switch (mode)
        {
        case SkyMode::CubeMap:

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

            break;

        case SkyMode::Shader:
        {
            if (shaderName.empty())
                return;

            Shader *shd = ShaderSystem::Get(shaderName);

            if (!shd)
            {
                ShaderSystem::Load(shaderName);
                shd = ShaderSystem::Get(shaderName);

                if (!shd)
                {
                    CE_ERROR("Sky::Render: Shader '%s' is invalid.", shaderName.c_str());
                    return;
                }
            }

            shd->Use();
            shd->Mat4(camera->GetProjection(), "uProjection");
            shd->Mat4(camera->GetViewMatrix(), "uView");
            shd->Mat4(Matrix4::Translate(camera->GetPosition()), "uModel");

            // Wip Shader data
            for (auto data : shaderData)
            {
                switch (data->type)
                {

                case SkyShaderDataType::Vec2:
                {
                    shd->Vec2((Vector2 *)data->Data, data->Name.c_str());
                }

                case SkyShaderDataType::Vec3:
                {
                    shd->Vec3((Vector3 *)data->Data, data->Name.c_str());
                }
                break;
                case SkyShaderDataType::Vec4:
                {
                    shd->Vec4((Vector4 *)data->Data, data->Name.c_str());
                }
                break;
                case SkyShaderDataType::Color:
                {
                    Color *c = (Color *)data->Data;
                    shd->Vec4(c->r / 255, c->g / 255, c->b / 255, c->a / 255, data->Name.c_str());
                }
                break;

                case SkyShaderDataType::None:
                default:
                    CE_WARN("Sky::Render: Unknown ShaderData.Type.");
                    break;
                }
            }
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

    void Sky::AddShaderData(CeU32 dataSize, void *Data, SkyShaderDataType dataType, const char *name)
    {
        shaderData.push_back(new SkyShaderData(dataSize, Data, dataType, name));
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
            switch (o->type)

            {
            case SkyShaderDataType::Vec2:
            {
                Vector2 *other = (Vector2 *)o->Data;
                AddShaderData(sizeof(Vector2), new Vector2(other->x, other->y), SkyShaderDataType::Vec2, o->Name.c_str());
            }
            break;

            case SkyShaderDataType::Vec3:
            {
                Vector3 *other = (Vector3 *)o->Data;
                AddShaderData(sizeof(Vector3), new Vector3(other->x, other->y, other->z), SkyShaderDataType::Vec3, o->Name.c_str());
            }
            break;

            case SkyShaderDataType::Vec4:
            {
                Vector4 *other = (Vector4 *)o->Data;
                AddShaderData(sizeof(Vector4), new Vector4(other->x, other->y, other->z, other->w), SkyShaderDataType::Vec4, o->Name.c_str());
            }
            break;

            case SkyShaderDataType::Color:
            {
                Color *other = (Color *)o->Data;
                AddShaderData(sizeof(Color), new Color(other->r, other->g, other->b, other->a), SkyShaderDataType::Color, o->Name.c_str());
            }
            break;

            case SkyShaderDataType::None:
            default:
                break;
            }
        }
    }
}