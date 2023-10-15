#include "Engine.h"

#include "Logger.h"
#include "Window.h"
#include "Timer.h"
#include "Memory/CeMemory.h"

#include "Platform/Platform.h"

#include "Event/EventManager.h"

#include "Layer/LayerStack.h"
#include "Layer/ImGuiLayer.h"

#include "Renderer/Renderer.h"
#include "Renderer/Manager/TextureManager.h"

#include "Scene/World.h"

#include "Script/ScriptEngine.h"

#include <glfw/glfw3.h>

namespace Core
{
    static EngineConfiguration *GConfig;
    static Window *GWindowInstance;
    static Application *GApp;
    static CeUserDataStructure GUserData;

    bool Engine::ShouldTick()
    {
        if (!GWindowInstance)
            return false;

        return !GWindowInstance->ShouldClose();
    }

    void Engine::PreInit(Application *app, EngineConfiguration *config)
    {
        CE_ASSERT_IF(app == nullptr && "Engine::PreInit: Application is nullptr.");
        CE_ASSERT_IF(config == nullptr && "Engine::PreInit: Application is config.");

        GConfig = config;
        GApp = app;

        // Setup systems
        Logger::Init();
        EventManager::Init();
        LayerStack::Init();
        World::Init();
        ScriptEngine::Init();

        // Create window
        GWindowInstance = new Window(config->windowConfig);
        CE_ASSERT_IF(GWindowInstance == nullptr && "Engine::PreInit: Window unable to create.");

        // Post window
        ImGuiLayer::Init();

        // Setup renderer
        Renderer::Init();

        // Setup lib
        ScriptEngine::LoadGameLibrary("GameLibrary.dll");

        // Starting application after starting subsystems
        GApp->Init();

        CE_DEBUG("Engine::PreInit successful.");
    }

    void Engine::Init()
    {
        World::InitActive();
        CE_DEBUG("Engine::Init successful.");
    }

    void Engine::Start()
    {
        // NOTE: Removed because of editor
        //  World::StartActive(); Starting the scene will be done at runtime, editor doesn't work as expected

        CE_DEBUG("Engine::Start successful.");
    }

    void Engine::Update()
    {
        GWindowInstance->Update();
        GApp->Update();

        EventManager::Update();
        LayerStack::Update();
        World::UpdateActive();
    }

    void Engine::Render()
    {
        // Render with renderer
        Renderer::BeginFrame();
        Renderer::Render();
        GApp->Render();
        World::RenderActive();
        Renderer::EndFrame();

        // Draw everything to Screen Shader
        Renderer::DrawImageToScreen();

        ImGuiLayer::BeginFrame();
        LayerStack::RenderImGui();
        ImGuiLayer::EndFrame();
    }

    void Engine::Shutdown()
    {
        EventManager::Shutdown();
        ImGuiLayer::Shutdown();
        LayerStack::Destroy();
        World::Shutdown();
        ScriptEngine::Shutdown();

        Renderer::Shutdown();

        GApp->Shutdown();
        delete GWindowInstance;
        delete GApp;
        CE_DEBUG("Engine::Shutdown successful.");
    }

    float Engine::GetWindowAspect()
    {
        if (!GWindowInstance)
            return 1280.0f / 720.0f;

        return (float)GWindowInstance->GetWidth() / (float)GWindowInstance->GetHeight();
    }

    Window *Engine::GetWindowInstance()
    {
        return GWindowInstance;
    }

    float Engine::GetTime()
    {
        return (float)glfwGetTime();
    }

    void Engine::GenerateUserData(void *data, CeU64 size)
    {
        GUserData.Data = CeMemory::Allocate(size);
        CeMemory::Copy(GUserData.Data, data, size);
        GUserData.Size = size;
    }

    void Engine::RegenerateUserData(void *data, CeU64 size)
    {
        CeMemory::Free(GUserData.Data);
        GenerateUserData(data, size);
    }

    void *Engine::GetUserData()
    {
        return GUserData.Data;
    }

    CeU64 Engine::GetUserDataSize()
    {
        return GUserData.Size;
    }

    void *Engine::GetCopyOfUserData()
    {
        void *block = CeMemory::Allocate(GUserData.Size);
        CeMemory::Copy(block, GUserData.Data, GUserData.Size);
        return block;
    }
}