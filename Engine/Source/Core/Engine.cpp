#include "Engine.h"

#include "Logger.h"
#include "Window.h"
#include "Timer.h"

#include "Event/EventManager.h"

#include "Layer/LayerStack.h"
#include "Layer/ImGuiLayer.h"

#include "Renderer/Renderer.h"
#include "Renderer/Manager/TextureManager.h"

#include <glfw/glfw3.h>

namespace Core
{
    static EngineConfiguration *GConfig;
    static Window *GWindowInstance;
    static Application *GApp;

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

        // Create window
        GWindowInstance = new Window(config->windowConfig);

        // Post window
        ImGuiLayer::Init();

        // Setup renderer
        Renderer::Init();

        // Starting application after starting subsystems
        GApp->Init();

        CE_DEBUG("Engine::PreInit successful.");
    }

    void Engine::Init()
    {
        CE_DEBUG("Engine::Init successful.");
    }

    void Engine::Start()
    {
        CE_DEBUG("Engine::Start successful.");
    }

    void Engine::Update()
    {
        GWindowInstance->Update();
        GApp->Update();

        EventManager::Update();
        LayerStack::Update();
    }

    void Engine::Render()
    {
        GApp->Render();

        // Render with renderer
        Renderer::BeginFrame();
        Renderer::Render();
        GApp->Render();
        Renderer::EndFrame();

        // Draw everything to Screen Shader
        Renderer::DrawImageToScreen();

        ImGuiLayer::BeginFrame();
        LayerStack::RenderImGui();
        ImGuiLayer::EndFrame();
    }

    void Engine::Shutdown()
    {
        CE_DEBUG("Engine::Shutdown entered.");

        EventManager::Shutdown();
        Renderer::Shutdown();
        ImGuiLayer::Shutdown();
        LayerStack::Destroy();

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
}