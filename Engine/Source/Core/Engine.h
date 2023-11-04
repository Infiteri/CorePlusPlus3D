#pragma once

#include "Base.h"
#include "Application.h"
#include "Core/CeConfigurations.h"
#include "Core/Window.h"
#include "Timer.h"

#include "Event/Event.h"

#include <vector>

namespace Core
{
    /// @brief Basic User Data struct with void* Data and CeU64 Size that represents the size of the void* Data.
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

        /// @brief Returns whether or not the application is running.
        /// @return True or false.
        static bool ShouldTick();

        // --------- NO USER CALL ------------
        static void PreInit(Application *app, EngineConfiguration *config);
        static void Init();
        static void Start();
        static void Update();
        static void Render();
        static void Shutdown();
        static void ClearEventContext(Event *event);
        // ----------------------------------

        /// @brief Returns the windows aspect ratio.
        /// @return Float.
        static float GetWindowAspect();

        /// @brief Returns a pointer to the window in which the application is running.
        /// @return A pointer to a window.
        static Window *GetWindowInstance();

        /// @brief Returns time in seconds.
        /// @return Float for time.
        static float GetTime();

        /// @brief Generates user data.
        /// @param data The data instance to use. The memory gets copied so could be a reference. (&stackMemory).
        /// @param size The size of the data.
        static void GenerateUserData(void *data, CeU64 size);

        /// @brief Regenerates the user data.
        /// @param data The data instance to use. The memory gets copied so could be a reference. (&stackMemory).
        /// @param size The size of the data.
        static void RegenerateUserData(void *data, CeU64 size);

        /// @brief Returns a void* of the user data, use casting to get the right type.
        /// @return void*
        static void *GetUserData();

        /// @brief Returns the data size of the user generated data.
        /// @return Number of the data.
        static CeU64 GetUserDataSize();

        /// @brief Returns a copy of the user data.
        /// @return void*
        static void *GetCopyOfUserData();

        static float GetDeltaTime();
        static float GetFPS();
        static void SetWantedFPS(float fpsValue);
        static float GetWantedFPS();

        static void PushTimerInfoForThisFrame(TimerInfo info);
        static std::vector<TimerInfo> GetTimerInfoForThisFrame();
    };
}