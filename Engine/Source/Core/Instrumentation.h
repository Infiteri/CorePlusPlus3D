#pragma once

#include "Base.h"
#include "Timer.h"

#include <string>
#include <fstream>

namespace Core
{
    struct ProfileResult
    {
        std::string Name;
        float Start, End;

        ProfileResult(const std::string &n, float B, float E)
        {
            Name = n;
            Start = B;
            End = E;
        };
    };

    struct InstrumentationSession
    {
        std::string Name;

        InstrumentationSession(const std::string &n) { Name = n; };
    };

    class CE_API Instrumentation
    {
    private:
        InstrumentationSession *currentSession;
        std::ofstream outputStream;
        int profileCount = 0;

    public:
        Instrumentation();
        ~Instrumentation();

        void BeginSession(const std::string &name, const std::string &filepath = "Debug/Profile.json");
        void EndSession();

        void WriteProfile(const ProfileResult &result);

        void WriteHeader();
        void WriteFooter();

        static Instrumentation *Get();
    };
}

#if CE_PROFILE
#define CE_PROFILE_BEGIN_SESSION(name) Core::Instrumentation::Get()->BeginSession(name)
#define CE_PROFILE_BEGIN_SESSION_FILE(name, path) Core::Instrumentation::Get()->BeginSession(name, path)
#define CE_PROFILE_END_SESSION() Core::Instrumentation::Get()->EndSession()
#define CE_PROFILE_FUNCTION() CE_PROFILE_SCOPE(CE_FUNCTION_SIGNATURE)
#define CE_PROFILE_SCOPE(name) \
    Core::Timer timer##__LINE__ { name }
#else
#define CE_PROFILE_BEGIN_SESSION(name)
#define CE_PROFILE_BEGIN_SESSION_FILE(name, path)
#define CE_PROFILE_END_SESSION()
#define CE_PROFILE_FUNCTION()
#define CE_PROFILE_SCOPE(...)
#endif