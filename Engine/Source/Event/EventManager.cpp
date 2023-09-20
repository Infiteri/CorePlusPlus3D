#include "EventManager.h"
#include "Math/Math.h"

#include <vector>
#include <algorithm>

namespace Core
{
    static std::vector<EventDispatcher *> dispatchers;
    static std::vector<Event *> events;
    static int EventsPerFrame = 10;

    EventDispatcher::EventDispatcher()
    {
    }

    EventDispatcher::~EventDispatcher()
    {
    }

    void EventDispatcher::OnEvent(Event *event)
    {
    }

    void EventManager::Init()
    {
    }

    void EventManager::Shutdown()
    {
    }

    void EventManager::Update()
    {
        if (dispatchers.empty() || events.empty() || EventsPerFrame == 0)
            return;

        // Get the amount of events to be handled.
        int amountOfEvents = Math::Min(events.size(), EventsPerFrame);

        // Handle them
        for (int i = 0; i < amountOfEvents; i++)
        {
            Event *e = events[i];

            for (EventDispatcher *d : dispatchers)
                d->OnEvent(e);

            auto it = std::find(events.begin(), events.end(), e);

            if (it != events.end())
                events.erase(it);

            e->Clear();
        }
    }

    void EventManager::SendEvent(Event *event)
    {
        events.push_back(event);
    }

    void EventManager::SendEvent(void *context, CeU32 size, EventType type)
    {
        Event *event = new Event();
        event->SetContextInformation(context, size);
        event->SetTypeInformation(type);

        SendEvent(event);
    }

    void EventManager::RegisterDispatcher(EventDispatcher *dispatcher)
    {
        dispatchers.push_back(dispatcher);
    }
}