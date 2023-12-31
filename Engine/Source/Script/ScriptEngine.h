#pragma once

#include "Base.h"
#include "ActorScript.h"
#include "Platform/Platform.h"

#include <string>
#include <vector>

namespace Core
{
    struct ScriptInfo
    {
        ActorScript *script;
        std::string name;

        ScriptInfo(const std::string &n, ActorScript *inst)
        {
            name = n;
            script = inst;
        };
    };
    static std::vector<ScriptInfo *> scripts;

    class CE_API ScriptEngine
    {
    public:
        ScriptEngine(){};
        ~ScriptEngine(){};

        static void Init();
        static void Shutdown();
        static bool HasGameLibrary();

        static void UpdateRuntime();
        static void StartRuntime();
        static void StopRuntime();
        static void RegisterScript(const std::string &name, ActorScript *script, Actor *parent);
        static void RegisterScript(const std::string &name, const std::string &scriptLoadName, Actor *parent);
        static void DeleteScript(const std::string &name);
        static void ClearScriptList();

        static DynamicLibrary *GetLibrary();
        static void LoadGameLibrary(const std::string &name);
        static void UnloadLibrary();

        static ActorScript *GetScriptByNameT(const std::string &name);

        template <typename T>
        static T *GetScriptByName(const std::string &name)
        {
            return (T *)GetScriptByNameT(name);
        };
    };
};
