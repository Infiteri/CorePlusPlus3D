#include "LayerStack.h"

#include <vector>
#include <algorithm>

std::vector<Core::Layer *> layers;
std::vector<Core::Layer *>::iterator insert;

namespace Core
{
    void LayerStack::Init()
    {
        insert = layers.begin();
    }

    void LayerStack::Destroy()
    {
        for (Layer *layer : layers)
        {
            layer->OnDetach();
            delete layer;
        }
    }

    void LayerStack::Update()
    {
        for (Layer *layer : layers)
        {
            layer->OnUpdate();
        }
    }

    void LayerStack::RenderImGui()
    {
        for (Layer *layer : layers)
        {
            layer->OnImGuiRender();
        }
    }

    void LayerStack::PushLayer(Layer *layer)
    {
        layer->OnAttach();
        insert = layers.emplace(insert, layer);
    }

    void LayerStack::PopLayer(Layer *layer)
    {
        auto it = std::find(layers.begin(), layers.end(), layer);
        if (it != layers.end())
        {
            layers.erase(it);
            insert--;
        }

        layer->OnDetach();
    }
}