#pragma once

//? ------------------------------ Welcome to Core/Layer/LayerStack.h ----------------------------------
//? This is a system designed to allow flexibility and ease of use.
//? ----------------------------------------------------------------------------------------------------
//? API:
//?     LayerStack::PushLayer(Layer*); -> Adds a new layer, will call its on attach method and detach
//?                                       when its done with it.
//?
//?     LayerStack::PopLayer(Layer*); -> Pops a layer, deletes and detaches it.
//?
//?     LayerStack::OnEvent(Event*); -> Sends a event to all layers, to receive the event add the
//?                                     OnEvent function on the layer class.
//? ----------------------------------------------------------------------------------------------------

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

        /// @brief Notifies all the active layers about an event.
        /// @param event The event to be sent.
        static void OnEvent(Event *event);

        /// @brief Pushes a new layer to the Layer Stack.
        /// @param layer A pointer to a layer class to be pushed.
        static void PushLayer(Layer *layer);

        /// @brief Pops the provided layer.
        /// @param layer A pointer to a layer instance to be popped.
        static void PopLayer(Layer *layer);
    };
}