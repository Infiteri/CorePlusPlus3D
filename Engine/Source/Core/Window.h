#pragma once

typedef struct GLFWwindow GLFWwindow;

#include "Base.h"
#include "CeConfigurations.h"

namespace Core
{
    class CE_API Window
    {
    private:
        GLFWwindow *handle;

        WindowConfiguration config;

    public:
        Window(WindowConfiguration config);
        ~Window();

        void Update();
        bool ShouldClose();

        /// @brief Returns the windows width.
        /// @return Integer.
        int GetWidth();

        /// @brief Returns the windows height.
        /// @return Integer.
        int GetHeight();

        /// @brief Returns the GLFW handle of the window.
        /// @return GLFWwindow* of the handle.
        inline GLFWwindow *GetHandle() { return handle; };
    };
}