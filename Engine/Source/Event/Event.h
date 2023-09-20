#pragma once

#include "Base.h"

namespace Core
{
    /// @brief A bunch of different events
    enum class EventType
    {
        None = 0,
        MouseMove,
        MouseClick,
        KeyEvent,
        WindowResize
    };

    class CE_API Event
    {
    private:
        /// @brief The void* to be casted to the appropriate type.
        void *Context;
        CeU64 contextSize;

        EventType type;

    public:
        Event();
        ~Event();

        void SetContextInformation(void *newContext, CeU64 size);

        void SetTypeInformation(EventType _type);
        EventType GetType();
        void Clear();
        CeU64 GetContextSize();
        void *GetContext();

        /// @brief Use this and then call "delete context" after done using, this will do a call to memory copy and will allocate memory. Needs to be freed after. Also keep in mind that is uses the "contextSize" and not "sizeof(Context)", should be set.
        /// @return Void*
        void *CopyContext();
    };
}