#include "ScriptEngine.h"

#include "Core/Logger.h"

#include <unordered_map>
#include <algorithm>

namespace Core
{
    static std::unordered_map<std::string, ActorScript *> scripts;
    static DynamicLibrary library;
    static bool hasLibrary = false;

    typedef ActorScript *(*GetActorScriptPFN)();

    void ScriptEngine::Init()
    {
        hasLibrary = false;
    }

    void ScriptEngine::Shutdown()
    {
        ClearScriptList();
    }

    bool ScriptEngine::HasGameLibrary()
    {
        return hasLibrary;
    }

    void ScriptEngine::UpdateRuntime()
    {
        for (auto it = scripts.begin(); it != scripts.end(); it++)
        {
            auto script = it->second;
            script->OnUpdate();
        }
    }

    void ScriptEngine::StartRuntime()
    {
        for (auto it = scripts.begin(); it != scripts.end(); it++)
        {
            auto script = it->second;
            script->OnStart();

            CE_TRACE("Starting script '%s'.", it->first.c_str());
        }
    }

    void ScriptEngine::StopRuntime()
    {
        for (auto it = scripts.begin(); it != scripts.end(); it++)
        {
            auto script = it->second;
            script->OnStop();

            CE_TRACE("Stopping script '%s'.", it->first.c_str());
        }

        ClearScriptList();
    }

    void ScriptEngine::RegisterScript(const std::string &name, ActorScript *script, Actor *parent)
    {
        script->owner = parent;
        scripts[name] = script;

        CE_TRACE("Script registered '%s' to parent/owner '%s'.", name.c_str(), parent->GetName().c_str());
    }

    void ScriptEngine::RegisterScript(const std::string &name, const std::string &scriptLoadName, Actor *parent)
    {
        GetActorScriptPFN pfn = Platform::LibraryGetFunction<GetActorScriptPFN>(&library, scriptLoadName + "Create");

        if (pfn == NULL)
        {
            CE_FATAL("Unable to Load PFN for creating actor script.");
            return;
        }

        RegisterScript(name, pfn(), parent);
    }

    void ScriptEngine::DeleteScript(const std::string &name)
    {
        if (scripts[name] != nullptr)
        {
            delete scripts[name];
            scripts.erase(name);
        }
    }

    void ScriptEngine::ClearScriptList()
    {
        for (auto it = scripts.begin(); it != scripts.end(); it++)
        {
            auto script = it->second;
            script->OnStop();
            CE_TRACE("Clearing script from list '%s'.", it->first.c_str());

            delete script;
        }

        scripts.clear();
    }

    DynamicLibrary *ScriptEngine::GetLibrary()
    {
        return &library;
    }

    void ScriptEngine::LoadGameLibrary(const std::string &name)
    {
        library = Platform::CreateLibrary(name);

        if (library.valid)
            hasLibrary = true;
    }

    void ScriptEngine::UnloadLibrary()
    {
        Platform::DestroyLibrary(&library);
    }
}