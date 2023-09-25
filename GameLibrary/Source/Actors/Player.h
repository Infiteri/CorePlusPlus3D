#pragma once

#include "Core.h"

class __declspec(dllexport) PlayerScript : public Core::ActorScript
{
public:
    PlayerScript();
    ~PlayerScript();
};

extern "C" __declspec(dllexport) Core::ActorScript *PlayerScriptCreate() { return new PlayerScript(); };