#pragma once

#include "Core/Layer/Layer.h"

namespace Core
{
    class EditorLayer : public Layer
    {
    public:
        EditorLayer(){};
        ~EditorLayer(){};

        void OnAttach();
        void OnRender();
        void OnUpdate();
        void OnImGuiRender();
        void OnDetach();
    };
}