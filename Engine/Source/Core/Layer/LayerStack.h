#pragma once

#include "Base.h"
#include "Layer.h"

namespace Core
{
    class CE_API LayerStack
    {
    public:
        LayerStack(){};
        ~LayerStack(){};

        static void Init();
        static void Destroy();
        static void Update();
        static void RenderImGui();

        /// @brief Pushes a new layer to the Layer Stack.
        /// @param layer A pointer to a layer class to be pushed.
        static void PushLayer(Layer *layer);

        /// @brief Pops the provided layer.
        /// @param layer A pointer to a layer instance to be popped.
        static void PopLayer(Layer *layer);
    };
}