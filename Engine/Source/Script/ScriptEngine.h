#pragma once

#include "Base.h"
#include "ActorScript.h"

#include <string>

namespace Core
{
    class ScriptEngine
    {
    public:
        ScriptEngine(){};
        ~ScriptEngine(){};

        static void Init();
        static void Shutdown();

        static void UpdateRuntime();
        static void StartRuntime();
        static void StopRuntime();
        static void RegisterScript(const std::string &name, ActorScript *script, Actor *parent);
        static void DeleteScript(const std::string &name);
        static void ClearScriptList();
    };
}