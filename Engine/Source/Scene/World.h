#pragma once

#include "Base.h"
#include "Scene.h"

#include <string>

namespace Core
{
    class CE_API World
    {
    public:
        World() {}
        ~World() {}

        static void Init();
        static void Shutdown();

        static Scene *Get(const std::string &name);
        static Scene *GetActive();
        static Scene *Create(const std::string &name);
        static void Activate(const std::string &name);
        static void Activate(Scene *scene);

        static Scene *CopyActive();
        static void CopyToActive(Scene *scene);

        static void Delete(const std::string &name);

        static void InitActive();
        static void StartActive();
        static void UpdateActive();
        static void RenderActive();
        static void StopActive();
        static void DestroyActive();
    };
}