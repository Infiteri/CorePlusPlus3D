#pragma once

#include "Base.h"

namespace Core
{
    class CE_API Layer
    {
    public:
        Layer();
        ~Layer();

        virtual void OnAttach();
        virtual void OnDetach();
        virtual void OnUpdate();
        virtual void OnImGuiRender();
    };
}