#include "Event.h"

#include "Platform/Platform.h"

namespace Core
{
    Event::Event()
    {
    }

    Event::~Event()
    {
        Clear();
    }

    void Event::SetContextInformation(void *newContext, CeU64 size)
    {
        Clear();

        Context = Platform::Allocate(size);
        Platform::MemCpy(Context, newContext, size);
        contextSize = size;
    }

    void Event::SetTypeInformation(EventType _type)
    {
        this->type = _type;
    }

    EventType Event::GetType()
    {
        return type;
    }

    void Event::Clear()
    {
        type = EventType::None;
        contextSize = 0;
    }

    CeU64 Event::GetContextSize()
    {
        return contextSize;
    }

    void *Event::GetContext()
    {
        return Context;
    }

    void *Event::CopyContext()
    {
        void *newBlock = Platform::Allocate(contextSize);

        Platform::MemCpy(newBlock, Context, contextSize) ;

        return newBlock;
    }
}