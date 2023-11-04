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
        CE_PROFILE_FUNCTION();

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
        CE_PROFILE_FUNCTION();

        for (auto &script : scripts)
        {
            script->script->OnUpdate();
        }
    }

    void ScriptEngine::StartRuntime()
    {
        CE_PROFILE_FUNCTION();

        for (auto &script : scripts)
        {
            script->script->OnStart();
        }
    }

    void ScriptEngine::StopRuntime()
    {
        CE_PROFILE_FUNCTION();

        for (auto &script : scripts)
        {
            script->script->OnStop();
        }

        ClearScriptList();
    }

    void ScriptEngine::RegisterScript(const std::string &name, ActorScript *script, Actor *parent)
    {
        CE_PROFILE_FUNCTION();

        script->owner = parent;
        scripts.push_back(new ScriptInfo(name, script));
    }

    void ScriptEngine::RegisterScript(const std::string &name, const std::string &scriptLoadName, Actor *parent)
    {
        CE_PROFILE_FUNCTION();

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
        CE_PROFILE_FUNCTION();

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
        CE_PROFILE_FUNCTION();

        for (auto &script : scripts)
        {
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
        CE_PROFILE_FUNCTION();

        library = Platform::CreateLibrary(name);

        if (library.valid)
            hasLibrary = true;
    }

    void ScriptEngine::UnloadLibrary()
    {
        CE_PROFILE_FUNCTION();

        Platform::DestroyLibrary(&library);
    }

    ActorScript *ScriptEngine::GetScriptByNameT(const std::string &name)
    {
        CE_PROFILE_FUNCTION();

        auto it = std::find_if(scripts.begin(), scripts.end(), [name](ScriptInfo *script)
                               { return (script->name == name); });

        if (it != scripts.end())
        {
            return (*it)->script;
        }

        return nullptr;
    }
}
