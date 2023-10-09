#include "Sky.h"

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
    }

    void Sky::Render()
    {
        // TODO: Shader

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

        case SkyMode::Color:
            glClearColor(color->r / 255, color->g / 255, color->b / 255, color->a / 255);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
        cubeTexture->Load(CubeMapConfigLoader::GetConfigFromFile(filepath));
    }
}