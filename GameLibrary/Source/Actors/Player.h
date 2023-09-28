#pragma once

#include "Core.h"

class __declspec(dllexport) PlayerScript : public Core::ActorScript
{
public:
    PlayerScript();
    ~PlayerScript();
};

extern "C" __declspec(dllexport) Core::ActorScript *PlayerScriptCreate() { return new PlayerScript(); };

class __declspec(dllexport) PlayerScript2 : public Core::ActorScript
{
public:
    PlayerScript2();
    ~PlayerScript2();
};

extern "C" __declspec(dllexport) Core::ActorScript *PlayerScript2Create() { return new PlayerScript2(); };