#include "Window.h"
#include "Input.h"
#include "Logger.h"

#include "Event/Event.h"
#include "Event/EventManager.h"
#include "Event/CeEvents.h"

#include "Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace Core
{
    void OnKeyEvent(GLFWwindow *win, int key, int scancode, int action, int mods);
    void OnButtonEvent(GLFWwindow *win, int button, int action, int mods);
    void OnMouseMove(GLFWwindow *win, double x, double y);
    void Viewport(GLFWwindow *win, int w, int h);

    Window::Window(WindowConfiguration config)
    {
        this->config = config;

        int result = glfwInit();
        CE_ASSERT_IF(result == GLFW_FALSE && "Window::Window: glfwInit didn't return successful.")

        if (!config.decorated)
        {
            glfwWindowHint(GLFW_DECORATED, false);
        }

        // Create window
        handle = glfwCreateWindow(config.width, config.height, config.title, NULL, NULL);
        CE_ASSERT_IF(handle == nullptr && "Window::Window: glfw window handle cannot be nullptr.")

        glfwSetWindowPos(handle, config.x, config.y);
        glfwMakeContextCurrent(handle);
        glfwSwapInterval(1);

        if (config.startMaximized)
        {
            glfwMaximizeWindow(handle);
            glfwGetWindowSize(handle, &this->config.width, &this->config.height);
            Renderer::Resize(this->config.width, this->config.height);
        }

        // Poll events
        glfwSetKeyCallback(handle, OnKeyEvent);
        glfwSetMouseButtonCallback(handle, OnButtonEvent);
        glfwSetCursorPosCallback(handle, OnMouseMove);
        glfwSetWindowSizeCallback(handle, Viewport);
    }

    Window::~Window()
    {
        glfwDestroyWindow(handle);
        glfwTerminate();
    }

    void Window::Update()
    {
        glfwSwapBuffers(handle);
        glfwPollEvents();

        // Update the mouse state
        double dx, dy;
        glfwGetCursorPos(handle, &dx, &dy);
        InputUpdateMouse((int)dx, (int)dy);

        glfwGetWindowSize(handle, &config.width, &config.height);
    }

    bool Window::ShouldClose()
    {
        return glfwWindowShouldClose(handle);
    }

    int Window::GetWidth()
    {
        return config.width;
    }

    int Window::GetHeight()
    {
        return config.height;
    }

    float Window::GetAspect()
    {
        return (float)config.width / (float)config.height;
    }

    void OnKeyEvent(GLFWwindow *win, int key, int scancode, int action, int mods)
    {
        bool pressed = action != GLFW_RELEASE;

        KeyEvent context;
        context.key = (Keys)key;

        //? Figure if its a press or release event
        if (action == GLFW_PRESS)
            context.type = KeyEventType::Press;
        else if (action == GLFW_REPEAT)
            context.type = KeyEventType::Repeat;
        else
            context.type = KeyEventType::Release;

        // Event will be cleaned by the manager
        Event *event = new Event();
        event->SetContextInformation(&context, sizeof(context));
        event->SetTypeInformation(EventType::KeyEvent);

        EventManager::SendEvent(event);

        InputUpdateKey((Keys)key, action != GLFW_RELEASE);
    }

    void OnButtonEvent(GLFWwindow *win, int button, int action, int mods)
    {
        bool pressed = action != GLFW_RELEASE;

        MouseClickEvent context;
        context.button = (Buttons)button;

        //? Figure if its a press or release event
        if (pressed)
            context.type = MouseEventType::Press;
        else
            context.type = MouseEventType::Release;
 
        // DONE: X / Y position
        context.x = Input::GetMouseX();
        context.y = Input::GetMouseY();

        EventManager::SendEvent(&context, sizeof(context), EventType::MouseClick);

        InputUpdateButton((Buttons)button, pressed);
    }

    void OnMouseMove(GLFWwindow *win, double x, double y)
    {
        // Note: Mouse x / y for input is in the Window::Update method

        MouseMoveEvent context;

        context.x = x;
        context.y = y;

        EventManager::SendEvent(&context, sizeof(context), EventType::MouseMove);
    }

    void Viewport(GLFWwindow *win, int w, int h)
    {
        // DONE: Event
        ResizeEvent context;
        context.width = w;
        context.height = h;

        Renderer::Resize(w, h);

        // NOTE: Viewport event might Resize Renderer so yeah must be done here haha
        EventManager::SendEvent(&context, sizeof(context), EventType::WindowResize);
    }
}