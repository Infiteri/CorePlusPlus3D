#include "ImGuiLayer.h"

#include "Core/Engine.h"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <ImGuizmo.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace Core
{
    void ImGuiLayer::Init()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        io.Fonts->AddFontFromFileTTF("EngineResources/Fonts/Open_Sans/static/OpenSans-Bold.ttf", 11.0f);

        // Styling
        ImGui::StyleColorsDark();

        // Will be started by the application, after the Engine::SetupWithApplication call
        ImGui_ImplGlfw_InitForOpenGL(Engine::GetWindowInstance()->GetHandle(), true);
        ImGui_ImplOpenGL3_Init("#version 330 core");

        auto &colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_WindowBg] = ImVec4{0.1f, 0.105f, 0.11f, 1.0f};

        // Headers
        colors[ImGuiCol_Header] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
        colors[ImGuiCol_HeaderHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
        colors[ImGuiCol_HeaderActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

        // Buttons
        colors[ImGuiCol_Button] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
        colors[ImGuiCol_ButtonHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
        colors[ImGuiCol_ButtonActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

        // Frame BG
        colors[ImGuiCol_FrameBg] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
        colors[ImGuiCol_FrameBgHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
        colors[ImGuiCol_FrameBgActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

        // Tabs
        colors[ImGuiCol_Tab] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
        colors[ImGuiCol_TabHovered] = ImVec4{0.38f, 0.3805f, 0.381f, 1.0f};
        colors[ImGuiCol_TabActive] = ImVec4{0.28f, 0.2805f, 0.281f, 1.0f};
        colors[ImGuiCol_TabUnfocused] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};

        // Title
        colors[ImGuiCol_TitleBg] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
        colors[ImGuiCol_TitleBgActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    }

    void ImGuiLayer::Shutdown()
    {
    }

    void ImGuiLayer::BeginFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();
    }

    void ImGuiLayer::EndFrame()
    {
        ImGuiIO &io = ImGui::GetIO();
        io.DisplaySize = ImVec2(Engine::GetWindowInstance()->GetWidth(), Engine::GetWindowInstance()->GetHeight());

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow *backup = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup);
        }
    }
}