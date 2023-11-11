#pragma once

#include "Panel.h"
#include <fstream>
#include <string>

namespace Core
{
    class ContentBrowserPanel : public Panel
    {
    public:
        std::string baseResourcesPath = ""; // TODO: From project
        std::string activePath = baseResourcesPath;
        ContentBrowserPanel();
        ~ContentBrowserPanel();

        void LoadAssets();

        void OnImGuiRender();

    private:
        void ZeroCharBuffers();
        void FOutFieldEqualValue(const char *filedName, const char *value, std::ofstream &out);
    };
}