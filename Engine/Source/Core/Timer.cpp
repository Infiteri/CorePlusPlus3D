#include "Timer.h"

#include "Core/Engine.h"
#include "Core/Logger.h"
#include "Core/Instrumentation.h"

namespace Core
{
    TimerInfo::TimerInfo()
    {
        Name = "";
        Time = 0;
    }

    TimerInfo::TimerInfo(Timer *timer)
    {
        Name = timer->GetName();
        Time = timer->GetTimeDiff();
    }

    TimerInfo::TimerInfo(Timer timer)
    {
        Name = timer.GetName();
        Time = timer.GetTimeDiff();
    }

    TimerInfo::TimerInfo(const char *n, float t)
    {
        Name = n;
        Time = t;
    }

    Timer::Timer(const char *_name)
    {
        name = _name;
        end = 0;
        begin = Engine::GetTime();
        stopped = false;
    }

    Timer::~Timer()
    {
        Stop();

        Engine::PushTimerInfoForThisFrame({this});
    }

    void Timer::Stop()
    {
        if (stopped)
            return;

        stopped = true;

        end = Engine::GetTime();

// Profile writing
#if CE_PROFILE
        Instrumentation::Get()->WriteProfile({name, begin * 1000, end * 1000});
#endif
    }

    float Timer::GetTimeDiff()
    {
        Stop();
        return end - begin;
    }

    const char *Timer::GetName()
    {
        return name;
    }

    void Timer::TraceTime(TimeType type)
    {
        Stop();

        int mul = 1;

        switch (type)
        {
        case Seconds:
            mul = 1;
            break;

        case Miliseconds:
        default:
            mul = 1000;
            break;
        }
        const char *ext = type == Seconds ? "S" : "MS";

        CE_CORE_TRACE("%s: %.3f %s.", name, (end - begin) * mul, ext);
    }
}