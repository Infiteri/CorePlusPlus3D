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
            {
                CE_WARN("Sky::Render: Shader name is empty. (SkyMode = Shader).");
                return;
            }

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

            // TODO: Basic matrices
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

    void Sky::From(Sky *other)
    {
        SetMode(other->GetMode());
        this->color->Set(other->GetColor());

        if (other->GetMode() == SkyMode::CubeMap)
            CreateCubeTexture(other->GetCubeTexturePath());
        else if (other->GetMode() == SkyMode::Shader)
            SetShaderName(other->GetShaderName());
    }
}