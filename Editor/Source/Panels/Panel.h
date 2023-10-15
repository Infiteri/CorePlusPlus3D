#pragma once

#include <imgui.h>

namespace Core
{
    class Panel
    {
    public:
        Panel(){};
        ~Panel(){};
        virtual void OnImGuiRender(){};
    };
}