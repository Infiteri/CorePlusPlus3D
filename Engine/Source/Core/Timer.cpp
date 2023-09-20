#include "Timer.h"

#include "Core/Engine.h"

namespace Core
{
    Timer::Timer(const char *_name)
    {
        name = _name;
        end = 0;
        begin = Engine::GetTime();
    }

    Timer::~Timer()
    {
        Stop();
    }

    void Timer::Stop()
    {
        if (stopped)
            return;

        stopped = true;

        end = Engine::GetTime();
    }
}