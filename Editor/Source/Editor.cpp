#include "Core.h"
#include "EditorLayer.h"
#include "Core/Launch/Launch.h"

#include <imgui.h>

namespace Core
{
    class Editor : public Application
    {
    public:
        Editor(){};

        void Init()
        {
            LayerStack::PushLayer(new EditorLayer());
        };
    };

    Application *CreateApplication(EngineConfiguration *config)
    {
        config->windowConfig.title = "Core Editor";
        config->windowConfig.startMaximized = true;
        
        return new Editor();
     };
}