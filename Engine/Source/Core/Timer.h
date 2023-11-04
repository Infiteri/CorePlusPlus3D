#pragma once

#include "Base.h"

namespace Core
{
    class Timer;

    struct TimerInfo
    {
        const char *Name;
        float Time;

        CE_API TimerInfo();
        CE_API TimerInfo(Timer *timer);
        CE_API TimerInfo(Timer timer);
        CE_API TimerInfo(const char *n, float t);
    };

    class CE_API Timer
    {
    private:
        const char *name;
        float begin;
        float end;
        bool stopped;

    public:
        enum TimeType
        {
            Seconds,
            Miliseconds,
        };

        Timer(const char *_name);
        ~Timer();

        /// @brief Call this to calculate the time differences. Stops the timer
        void Stop();

        /// @brief Return the time differences, has to call stop if it hasn't been called.
        /// @return Float for time difference. (end - begin)
        float GetTimeDiff();

        const char *GetName();

        /// @brief Prints a Trace message to console with format:
        /// "name: (%f)".
        /// @param msg
        void TraceTime(TimeType type = Seconds);
    };
}
