#pragma once

#include "Base.h"
#include "Application.h"
#include "Core/CeConfigurations.h"
#include "Core/Window.h"

namespace Core
{
    struct CeUserDataStructure
    {
        void *Data;
        CeU64 Size;
    };

    class CE_API Engine
    {
    public:
        Engine(){};
        ~Engine(){};

        static bool ShouldTick();

        static void PreInit(Application *app, EngineConfiguration *config);

        static void Init();
        static void Start();

        static void Update();
        static void Render();

        static void Shutdown();

        static float GetWindowAspect();

        static Window *GetWindowInstance();

        /// @brief Returns time in seconds.
        /// @return Float for time.
        static float GetTime();

        static void GenerateUserData(void *data, CeU64 size);
        static void RegenerateUserData(void *data, CeU64 size);
        static void *GetUserData();
        static CeU64 GetUserDataSize();
        static void *GetCopyOfUserData();
    };
}