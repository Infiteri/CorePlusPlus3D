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
    static DynamicLibrary testLibrary;

    bool Engine::ShouldTick()
    {
        if (!GWindowInstance)
            return false;

        return !GWindowInstance->ShouldClose();
    }

    void Engine::PreInit(Application *app, EngineConfiguration *config)
    {
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

        // Post window
        ImGuiLayer::Init();

        // Setup renderer
        Renderer::Init();

        // Starting application after starting subsystems
        GApp->Init();

        testLibrary = Platform::CreateLibrary("GameLibrary.dll");
        Platform::LibraryLoadFunction(&testLibrary, "GameInit");
        typedef void (*GameInit)();
        GameInit f = (GameInit)testLibrary.functions["GameInit"]->pfn;
        f();

        CE_DEBUG("Engine::PreInit successful.");
    }

    void Engine::Init()
    {
        World::InitActive();
        CE_DEBUG("Engine::Init successful.");
    }

    void Engine::Start()
    {
        World::StartActive();
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

        Platform::DestroyLibrary(&testLibrary);

        Renderer::Shutdown();

        GApp->Shutdown();
        delete GWindowInstance;
        delete GApp;

        CE_DEBUG("Engine::Shutdown successful.");
    }

    float Engine::GetWindowAspect()
    {
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