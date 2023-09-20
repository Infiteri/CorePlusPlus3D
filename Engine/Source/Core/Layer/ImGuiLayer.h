#pragma once

#include "Base.h"

namespace Core
{
    class ImGuiLayer
    {
    public:
        ImGuiLayer(){};
        ~ImGuiLayer(){};

        static void Init();
        static void Shutdown();
        static void BeginFrame();
        static void EndFrame();
    };
}