#include "ScriptEngine.h"

#include "Core/Logger.h"

#include <algorithm>

namespace Core
{
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
        for (auto &script : scripts)
        {
            script->script->OnUpdate();
        }
    }

    void ScriptEngine::StartRuntime()
    {
        for (auto &script : scripts)
        {
            script->script->OnStart();
            CE_TRACE("Starting script.");
        }
    }

    void ScriptEngine::StopRuntime()
    {
        for (auto &script : scripts)
        {
            script->script->OnStop();
            CE_TRACE("Stopping script.");
        }

        ClearScriptList();
    }

    void ScriptEngine::RegisterScript(const std::string &name, ActorScript *script, Actor *parent)
    {
        script->owner = parent;
        scripts.push_back(new ScriptInfo(name, script));

        CE_TRACE("Script registered to parent.");
    }

    void ScriptEngine::RegisterScript(const std::string &name, const std::string &scriptLoadName, Actor *parent)
    {
        GetActorScriptPFN pfn = Platform::LibraryGetFunction<GetActorScriptPFN>(&library, scriptLoadName + "Create");

        if (pfn == nullptr)
        {
            CE_FATAL("Unable to load PFN for creating actor script.");
            return;
        }

        RegisterScript(name, pfn(), parent);
    }

    void ScriptEngine::DeleteScript(const std::string &name)
    {
        for (auto &script : scripts)
        {
            if (script->name == name)
            {
                delete script->script;
                delete script;
            }
        }
    }

    void ScriptEngine::ClearScriptList()
    {
        for (auto &script : scripts)
        {
            CE_TRACE("Clearing script from list.");
            delete script->script;
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

    ActorScript *ScriptEngine::GetScriptByNameT(const std::string &name)
    {
        auto it = std::find_if(scripts.begin(), scripts.end(), [name](ScriptInfo *script)
                               { return (script->name == name); });

        if (it != scripts.end())
        {
            return (*it)->script;
        }

        return nullptr;
    }
}
