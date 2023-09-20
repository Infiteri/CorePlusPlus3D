#pragma once

typedef struct GLFWwindow GLFWwindow;

#include "Base.h"
#include "CeConfigurations.h"

namespace Core
{
    class Window
    {
    private:
        GLFWwindow *handle;

        WindowConfiguration config;

    public:
        Window(WindowConfiguration config);
        ~Window();

        void Update();
        bool ShouldClose();

        int GetWidth();
        int GetHeight();

        inline GLFWwindow *GetHandle() { return handle; };
    };
}