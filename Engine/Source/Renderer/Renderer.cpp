#include "Renderer.h"

#include "Core/Engine.h"
#include "Core/Logger.h"
#include "Core/Input.h"
#include "Core/Memory/CeMemory.h"

#include "Shader.h"
#include "Objects/Mesh.h"
#include "Buffer/FrameBuffer.h"
#include "Buffer/VertexArray.h"

#include "Math/Matrix4.h"
#include "Math/Math.h"

#include "Camera/PerspectiveCamera.h"
#include "Camera/PerspectiveMovement.h"
#include "Camera/CameraSystem.h"

#include "Manager/TextureManager.h"
#include "Manager/MaterialManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Core
{
    static float screenQuadVertices[] = {
        // positions   // texCoords
        -1.0f, 1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 0.0f,

        -1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f,

        1.0f, 1.0f, 1.0f, 1.0f};

    struct RendererState
    {
        Shader *ObjectShader;
        Shader *ScreenShader;

        Viewport viewport;
        Color backgroundColor;

        // Screen related

        FrameBuffer *ScreenFramebuffer;
        VertexArray *ScreenVertexArray;

        // End screen related

        // TODO: User defined
        PerspectiveMovement *movement;

        GLenum depthType = GL_LEQUAL;
    };

    static RendererState *state;

    void Renderer::Init()
    {
        // ! LOAD GLAD
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        // ! END LOAD GLAD

        // Setup systems related to rendering
        TextureManager::Init();
        MaterialManager::Init();
        CameraSystem::Init();

        // Allocate the State
        state = (RendererState *)CeMemory::Allocate(sizeof(RendererState));
        CeMemory::TracePrintSize("RendererState: ", sizeof(RendererState));

        // Setup viewport structure
        state->viewport.width = Engine::GetWindowInstance()->GetWidth();
        state->viewport.height = Engine::GetWindowInstance()->GetHeight();

        // Load up shaders
        state->ObjectShader = new Shader("EngineResources/Shaders/Object.vs.glsl", "EngineResources/Shaders/Object.fs.glsl");
        state->ScreenShader = new Shader("EngineResources/Shaders/Screen.vs.glsl", "EngineResources/Shaders/Screen.fs.glsl");

        FrameBufferSpecification spec;
        spec.width = state->viewport.width;
        spec.height = state->viewport.height;

        spec.renderPassSpecificationCount = 2;
        spec.renderPassSpecifications = (RenderPassSpecification *)CeMemory::Allocate(sizeof(RenderPassSpecification) * spec.renderPassSpecificationCount);
        spec.renderPassSpecifications[0].textureType = RenderPassTextureType::Rgba8;
        spec.renderPassSpecifications[1].textureType = RenderPassTextureType::Depth;

        state->ScreenFramebuffer = new FrameBuffer(spec);

        state->ScreenVertexArray = new VertexArray();
        state->ScreenVertexArray->GenVertexBuffer(screenQuadVertices, sizeof(screenQuadVertices)); // ! NO sizeof(float) cuz it makes the final image broken (? IDEK Why);
        state->ScreenVertexArray->GetVertexBuffer()->AddLayout(0, 0, 2);
        state->ScreenVertexArray->GetVertexBuffer()->AddLayout(1, 2, 2);

        state->backgroundColor.Set(0, 0, 0, 255);

        state->movement = new PerspectiveMovement();

        // After objects creation
        RegenerateObjectsWithNewViewport();
    }

    void Renderer::Shutdown()
    {
        MaterialManager::Shutdown();
        TextureManager::Shutdown();
        CameraSystem::Shutdown();
        CeMemory::Free(state);
    }

    void Renderer::BeginFrame()
    {
        state->ScreenFramebuffer->Bind();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(state->depthType);

        Color *bg = &state->backgroundColor;

        // TODO: User Color / Sky rendering
        glClearColor(bg->r / 255, bg->g / 255, bg->b / 255, bg->a / 255);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::Render()
    {
        state->ObjectShader->Use();

        // TODO: Scene rendering

        PerspectiveCamera *camera = CameraSystem::GetActive();
        if (camera)
        {
            state->movement->Update(camera);
            state->ObjectShader->Mat4(camera->GetProjection(), "uProjection");
            Matrix4 upload = camera->GetViewMatrix();
            state->ObjectShader->Mat4(upload, "uView");
        }
    }

    void Renderer::EndFrame()
    {
        state->ScreenFramebuffer->Unbind();

        glDisable(GL_DEPTH_TEST);

        // If green - this is wrong
        glClearColor(0, 1, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Renderer::RegenerateObjectsWithNewViewport()
    {
        CameraSystem::UpdateProjectionForActiveCamera((float)state->viewport.width / (float)state->viewport.height);

        state->ScreenFramebuffer->Resize(state->viewport.width, state->viewport.height);

        glViewport(0, 0, state->viewport.width, state->viewport.height);
    }

    void Renderer::DrawImageToScreen()
    {
        state->ScreenShader->Use();

        state->ScreenVertexArray->Bind();

        // Bind texture
        RenderPassSpecification *renderPass = state->ScreenFramebuffer->GetRenderPass(0);
        glActiveTexture(GL_TEXTURE0 + renderPass->index);
        glBindTexture(GL_TEXTURE_2D, renderPass->id);
        state->ScreenShader->Int(renderPass->index, "uScreenTexture");

        state->ScreenVertexArray->GetVertexBuffer()->Bind();
        state->ScreenVertexArray->GetVertexBuffer()->Draw();
    }

    void Renderer::Resize(CeU64 w, CeU64 h)
    {
        if (!state)
            return;

        // Set viewport sizes
        state->viewport.width = w;
        state->viewport.height = h;

        //! POST VIEWPORT SET
        RegenerateObjectsWithNewViewport();
    }

    Shader *Renderer::GetObjectShader()
    {
        return state->ObjectShader;
    }

    Color *Renderer::GetBackgroundColor()
    {
        return &state->backgroundColor;
    }

    void Renderer::SetDepthMode(DepthMode mode)
    {
        switch (mode)
        {
        case DepthMode::Always:
            state->depthType = GL_ALWAYS;
            break;

        case DepthMode::Less:
            state->depthType = GL_LESS;
            break;

        case DepthMode::Lequal:
        default:
            state->depthType = GL_LEQUAL;
            break;
        }
    }

    void Renderer::SetBackgroundColor(float r, float g, float b, float a)
    {
        state->backgroundColor.Set(r, g, b, a);
    }
}
