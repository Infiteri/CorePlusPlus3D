#pragma once

#include "Base.h"
#include "Event.h"

namespace Core
{
    class CE_API EventDispatcher
    {
    public:
        EventDispatcher();
        ~EventDispatcher();

        virtual void OnEvent(Event *event);
    };

    class CE_API EventManager
    {
    public:
        EventManager(){};
        ~EventManager(){};

        static void Init();
        static void Shutdown();
        static void Update();

        static void SendEvent(Event *event);
        static void SendEvent(void* context, CeU32 size, EventType type);
        static void RegisterDispatcher(EventDispatcher *dispatcher);
    };
}