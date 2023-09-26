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
        return new Editor();
    };
}