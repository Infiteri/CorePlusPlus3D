#include "Platform.h"

#ifdef CE_WIN32

#include "Core/Engine.h"

#include <windows.h>
#include <tchar.h>
#include <Shlwapi.h>
#include <commdlg.h>
#include <shlwapi.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

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

    std::vector<std::string> Platform::GetFilePathsInDirectory(const std::string &directoryPath)
    {
        std::vector<std::string> filePaths;

        WIN32_FIND_DATA findData;
        HANDLE hFind = FindFirstFile((directoryPath + "/*").c_str(), &findData);

        if (hFind == INVALID_HANDLE_VALUE)
        {
            return filePaths;
        }

        do
        {
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                filePaths.push_back(directoryPath + "/" + findData.cFileName);
            }
        } while (FindNextFile(hFind, &findData));

        FindClose(hFind);

        return filePaths;
    }

    std::vector<std::string> Platform::GetFolderPathsInDirectory(const std::string &directoryPath)
    {
        std::vector<std::string> folderNames;

        WIN32_FIND_DATA findData;
        HANDLE hFind = FindFirstFile((directoryPath + "/*").c_str(), &findData);

        if (hFind == INVALID_HANDLE_VALUE)
        {
            return folderNames;
        }

        do
        {
            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                std::string folderName = findData.cFileName;
                if (folderName != "." && folderName != "..")
                {
                    folderNames.push_back(folderName);
                }
            }
        } while (FindNextFile(hFind, &findData));

        FindClose(hFind);

        return folderNames;
    }

    std::vector<DirectoryEntry> Platform::GetDirectoryEntries(const std::string &directoryPath)
    {
        std::vector<DirectoryEntry> paths;
        std::vector<std::string> filePaths = GetFilePathsInDirectory(directoryPath);
        std::vector<std::string> folderPaths = GetFolderPathsInDirectory(directoryPath);

        for (std::string fP : folderPaths)
        {
            DirectoryEntry p;
            p.path = fP;
            p.isFolder = true;

            paths.emplace_back(p);
        }

        for (std::string fP : filePaths)
        {
            DirectoryEntry p;
            p.path = fP;
            p.isFolder = false;

            paths.emplace_back(p);
        }

        return paths;
    }

    bool Platform::CreateFolder(const std::string &name)
    {
        return CreateDirectory(name.c_str(), nullptr) != 0;
    }

    std::string Platform::OpenFileDialog(const char *filter)
    {
        OPENFILENAMEA ofn;
        CHAR szFile[260] = {0};
        CHAR currentDir[256] = {0};
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = glfwGetWin32Window((GLFWwindow *)Engine::GetWindowInstance()->GetHandle());
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        if (GetCurrentDirectoryA(256, currentDir))
            ofn.lpstrInitialDir = currentDir;

        ofn.lpstrFilter = filter;
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        if (GetOpenFileNameA(&ofn) == TRUE)
            return ofn.lpstrFile;

        return std::string();
    }

    std::string Platform::SaveFileDialog(const char *filter)
    {
        OPENFILENAMEA ofn;
        CHAR szFile[260] = {0};
        CHAR currentDir[256] = {0};
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = glfwGetWin32Window((GLFWwindow *)Engine::GetWindowInstance()->GetHandle());
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        if (GetCurrentDirectoryA(256, currentDir))
            ofn.lpstrInitialDir = currentDir;
        ofn.lpstrFilter = filter;
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

        // Sets the default extension by extracting it from the filter
        ofn.lpstrDefExt = strchr(filter, '\0') + 1;

        if (GetSaveFileNameA(&ofn) == TRUE)
            return ofn.lpstrFile;

        return std::string();
    }

}

#endif