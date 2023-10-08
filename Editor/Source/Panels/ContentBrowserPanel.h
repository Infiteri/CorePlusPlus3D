#pragma once

#include <imgui.h>

namespace Core
{
    class ContentBrowserPanel
    {
    public:
        ContentBrowserPanel();
        ~ContentBrowserPanel();

        void LoadAssets();
        
        void OnImGuiRender();
    };
}