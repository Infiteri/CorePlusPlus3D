#pragma once

#include "Base.h"
#include "Core/Logger.h"

#include <string>
#include <unordered_map>

namespace Core
{
    enum class PlatformLogColor
    {
        White,
        Red,
        DarkRed,
        Blue,
        Gray,
        Green,
        Yellow,
    };

    struct DynamicLibraryFunction
    {
        std::string name;
        void *pfn;
    };

    struct DynamicLibrary
    {
        std::string name;
        bool valid = false;
        void *internal;
        std::unordered_map<std::string, DynamicLibraryFunction *> functions;
    };

    class CE_API Platform
    {
    public:
        Platform(){};
        ~Platform(){};

        static void SetConsoleColor(PlatformLogColor color);
        static void LogMessage(const char *msg);

        // -------------------- MEMORY --------------------

        static void *MemSet(void *block, int value, CeU64 size);
        static void Free(void *block);
        static void *Allocate(CeU64 size);
        static void *MemCpy(void *dest, const void *source, CeU64 size);

        // ------------------------------------------------

        // --------------- DYNAMIC LIBRARY ---------------

        static DynamicLibrary CreateLibrary(const std::string &_name);
        static bool LibraryLoadFunction(DynamicLibrary *library, const std::string &functionName);
        static void DestroyLibrary(DynamicLibrary *library);

        template <typename T>
        static T LibraryGetFunction(DynamicLibrary *library, const std::string &funcName)
        {
            if (!library || !library->valid)
            {
                CE_ERROR("Library not valid when searching for function.");
                return NULL;
            }

            if (!library->functions[funcName])
            {
                if (!LibraryLoadFunction(library, funcName))
                {
                    CE_ERROR("Unable to load function for library when trying to get it.");
                    return NULL;
                }
            }

            return (T)library->functions[funcName]->pfn;
        };

        // ------------------------------------------------
    };

}