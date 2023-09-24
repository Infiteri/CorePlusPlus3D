#include "ScriptEngine.h"

#include "Core/Logger.h"

#include <unordered_map>
#include <algorithm>

namespace Core
{
    static std::unordered_map<std::string, ActorScript *> scripts;

    void ScriptEngine::Init()
    {
    }

    void ScriptEngine::Shutdown()
    {
        ClearScriptList();
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
}