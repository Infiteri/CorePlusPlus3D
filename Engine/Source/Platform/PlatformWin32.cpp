#include "Platform.h"

#ifdef CE_WIN32

#include <windows.h>

namespace Core
{
    void Platform::SetConsoleColor(PlatformLogColor color)
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        WORD colorAttribute = FOREGROUND_INTENSITY; // Default to high-intensity (bright) text

        switch (color)
        {
        case PlatformLogColor::White:
            colorAttribute |= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
            break;

        case PlatformLogColor::Gray:
            colorAttribute |= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
            break;

        case PlatformLogColor::Red:
            colorAttribute |= FOREGROUND_RED;
            break;

        case PlatformLogColor::DarkRed:
            colorAttribute |= FOREGROUND_RED;
            break;

        case PlatformLogColor::Blue:
            colorAttribute |= FOREGROUND_BLUE;
            break;

        case PlatformLogColor::Green:
            colorAttribute |= FOREGROUND_GREEN;
            break;

        case PlatformLogColor::Yellow:
            colorAttribute |= FOREGROUND_RED | FOREGROUND_GREEN;
            break;

        default:
            // Default to white if an unsupported color is specified
            colorAttribute |= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
            break;
        }

        SetConsoleTextAttribute(hConsole, colorAttribute);
    }

    void Platform::LogMessage(const char *msg)
    {
        OutputDebugStringA(msg);

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD bytesWritten;

        WriteConsoleA(hConsole, msg, strlen(msg), &bytesWritten, NULL);
    }

    void *Platform::MemSet(void *block, int value, CeU64 size)
    {
        return memset(block, value, size);
    }

    void Platform::Free(void *block)
    {
        free(block);
    }

    void *Platform::Allocate(CeU64 size)
    {

        return malloc(size);
    }

    void *Platform::MemCpy(void *dest, const void *source, CeU64 size)
    {
        return memcpy(dest, source, size);
    }

    DynamicLibrary Platform::CreateLibrary(const std::string &_name)
    {
        DynamicLibrary out;
        if (_name.empty())
        {
            out.valid = false;
            CE_FATAL("Unable to create a library with no name.");
            return out;
        }

        // Load to make sure its fine
        HMODULE mod = LoadLibrary(_name.c_str());

        if (mod == NULL)
        {
            out.valid = false;
            CE_FATAL("Unable to create a library with name '%s'.", _name.c_str());
            return out;
        }

        // Set
        out.internal = mod;
        out.valid = true;
        out.name = _name;

        return out;
    }

    bool Platform::LibraryLoadFunction(DynamicLibrary *library, const std::string &functionName)
    {
        if (!library || !library->valid)
            return false;

        FARPROC f_addr = GetProcAddress((HMODULE)library->internal, functionName.c_str());
        if (!f_addr)
            return false;

        DynamicLibraryFunction *f = new DynamicLibraryFunction;
        f->pfn = (void *)f_addr;
        f->name = functionName;
        library->functions[functionName] = f;

        return true;
    }

    void Platform::DestroyLibrary(DynamicLibrary *library)
    {
        if (!library || !library->valid || library->name.empty())
            return;

        for (auto it = library->functions.begin(); it != library->functions.end(); it++)
        {
            delete it->second;
        }

        library->functions.clear();

        FreeLibrary((HMODULE)library->internal);
    }
}

#endif