#include "Engine.h"

#include "Logger.h"
#include "Window.h"
#include "Instrumentation.h"
#include "Memory/CeMemory.h"

#include "Platform/Platform.h"

#include "Event/CeEvents.h"
#include "Event/EventManager.h"

#include "Layer/LayerStack.h"
#include "Layer/ImGuiLayer.h"

#include "Renderer/Renderer.h"
#include "Renderer/Manager/TextureManager.h"

#include "Scene/World.h"

#include "Project/Project.h"

#include "Script/ScriptEngine.h"

#include <glfw/glfw3.h>

#define CE_CLEAR_CONTEXT_UTILS(a, b)             \
    if (event->GetType() == a)                   \
    {                                            \
        b *OUT_EVENT = (b *)event->GetContext(); \
        delete OUT_EVENT;                        \
    }

namespace Core
{
    static EngineConfiguration *GConfig;
    static Window *GWindowInstance;
    static Application *GApp;
    static CeUserDataStructure GUserData;
    static float lastTime;
    static float deltaTime;
    static std::vector<TimerInfo> timerInfos;

    // Dont skip frames
    static float wantedFPS = 0.0f;

    bool Engine::ShouldTick()
    {
        if (!GWindowInstance)
            return false;

        return !GWindowInstance->ShouldClose();
    }

    void Engine::PreInit(Application *app, EngineConfiguration *config)
    {
        CE_PROFILE_FUNCTION();

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

        // Load Project
        // Note:: In core every project will have a file named "Project.ce_proj".
        Project::New();
        Project::Load("Project.ce_proj");

        if (Project::GetConfig() == nullptr)
        {
            //? Debug mode only
            CE_ASSERT_IF(Project::GetConfig() == nullptr && "Project name must be Project.ce_proj");
            //? Debug mode only end

            Engine::Shutdown();
            return;
        }

        // Setup lib
        if (!Project::GetConfig()->scriptPath.empty())
            ScriptEngine::LoadGameLibrary(Project::GetConfig()->scriptPath);

        // Starting application after starting subsystems
        GApp->Init();

        CE_DEBUG("Engine::PreInit successful.");
    }

    void Engine::Init()
    {
        CE_PROFILE_FUNCTION();

        World::InitActive();
        CE_DEBUG("Engine::Init successful.");
    }

    void Engine::Start()
    {
        CE_PROFILE_FUNCTION();

        // NOTE: Removed because of editor
        //  World::StartActive(); Starting the scene will be done at runtime, editor doesn't work as expected

        CE_DEBUG("Engine::Start successful.");
    }

    void Engine::Update()
    {
        CE_PROFILE_FUNCTION();

        double currentFrameTime = glfwGetTime();
        deltaTime = static_cast<float>(currentFrameTime - lastTime);
        lastTime = static_cast<float>(currentFrameTime);

        GWindowInstance->Update();
        GApp->Update();

        EventManager::Update();
        LayerStack::Update();
        World::UpdateActive();
    }

    void Engine::Render()
    {
        CE_PROFILE_FUNCTION();
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

        timerInfos.clear();
    }

    void Engine::Shutdown()
    {
        CE_PROFILE_FUNCTION();

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

    void Engine::ClearEventContext(Event *event)
    {
        CE_CLEAR_CONTEXT_UTILS(EventType::MouseMove, MouseMoveEvent);
        CE_CLEAR_CONTEXT_UTILS(EventType::KeyEvent, KeyEvent);
        CE_CLEAR_CONTEXT_UTILS(EventType::MouseClick, MouseClickEvent);
        CE_CLEAR_CONTEXT_UTILS(EventType::WindowResize, ResizeEvent);
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

    float Engine::GetDeltaTime()
    {
        return deltaTime;
    }

    float Engine::GetFPS()
    {
        return 1.0f / deltaTime;
    }

    void Engine::SetWantedFPS(float fpsValue)
    {
        wantedFPS = fpsValue;
    }

    float Engine::GetWantedFPS()
    {
        return wantedFPS;
    }

    void Engine::PushTimerInfoForThisFrame(TimerInfo info)
    {
        timerInfos.push_back(info);
    }

    std::vector<TimerInfo> Engine::GetTimerInfoForThisFrame()
    {
        return timerInfos;
    }
}