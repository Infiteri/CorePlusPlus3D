#pragma once

#include "Base.h"

namespace Core
{
    class CE_API Timer
    {
    private:
        const char *name;
        float begin;
        float end;
        bool stopped;

    public:
        Timer(const char *_name);
        ~Timer();

        /// @brief Call this to calculate the time differences.
        void Stop();

        /// @brief Return the time differences, has to call stop if it hasn't been called.
        /// @return Float for time difference. (end - begin)
        inline float GetTimeDiff()
        {
            if (!stopped)
                Stop();

            return end - begin;
        };
    };
}