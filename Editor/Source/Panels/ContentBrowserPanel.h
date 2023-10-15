#pragma once

#include "Panel.h"
#include <fstream>

namespace Core
{
    class ContentBrowserPanel : public Panel
    {
    public:
        ContentBrowserPanel();
        ~ContentBrowserPanel();

        void LoadAssets();

        void OnImGuiRender();

    private:
        void ZeroCharBuffers();
        void FOutFieldEqualValue(const char *filedName, const char *value, std::ofstream &out);
    };
}