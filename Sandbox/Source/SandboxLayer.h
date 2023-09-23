#pragma once

#include "Core.h"

class SandboxLayer : public Core::Layer
{
public:
    SandboxLayer(){};
    ~SandboxLayer(){};

    void OnImGuiRender();
};
