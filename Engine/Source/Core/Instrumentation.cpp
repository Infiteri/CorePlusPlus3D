#include "Instrumentation.h"
#include "Core/Logger.h"

#include <algorithm>

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

#include <thread>

namespace Core
{
    static Instrumentation *inst = new Instrumentation();

    Instrumentation::Instrumentation()
    {
        currentSession = nullptr;
        profileCount = 0;
    }

    Instrumentation::~Instrumentation()
    {
    }

    void Instrumentation::BeginSession(const std::string &name, const std::string &filepath)
    {
        outputStream.open(filepath);
        WriteHeader();
        currentSession = new InstrumentationSession(name);
    }

    void Instrumentation::EndSession()
    {
        WriteFooter();
        outputStream.close();
        delete currentSession;
        currentSession = nullptr;
        profileCount = 0;
    }

    void Instrumentation::WriteProfile(const ProfileResult &result)
    {
        if (profileCount > 0)
            outputStream << ",";

        profileCount++;

        std::string name = result.Name;
        std::replace(name.begin(), name.end(), '"', '\'');
        outputStream << "{";
        outputStream << "\"cat\":\"function\",";
        outputStream << "\"dur\":" << (result.End - result.Start) << ',';
        outputStream << "\"name\":\"" << name << "\",";
        outputStream << "\"ph\":\"X\",";
        outputStream << "\"pid\":0,";
        outputStream << "\"tid\":" << 0 << ",";
        outputStream << "\"ts\":" << result.Start;
        outputStream << "}";

        outputStream.flush();
    }

    void Instrumentation::WriteHeader()
    {
        outputStream << "{\"otherData\": {},\"traceEvents\":[";
        outputStream.flush();
    }

    void Instrumentation::WriteFooter()
    {
        outputStream << "]}";
        outputStream.flush();
    }

    Instrumentation *Instrumentation::Get()
    {
        return inst;
    }
}
