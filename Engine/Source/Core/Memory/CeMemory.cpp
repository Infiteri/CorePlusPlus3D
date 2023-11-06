#include "CeMemory.h"

#include "Platform/Platform.h"
#include "Core/Logger.h"

namespace Core
{
    void *CeMemory::Allocate(CeU64 size)
    {
        return Platform::Allocate(size);
    }

    void *CeMemory::Copy(void *Block, const void *Source, CeU64 size)
    {
        return Platform::MemCpy(Block, Source, size);
    }

    void *CeMemory::Set(void *Block, CeU32 value, CeU64 size)
    {
        return Platform::MemSet(Block, value, size);
    }

    void CeMemory::Free(void *Block)
    {
        Platform::Free(Block);
    }

    void CeMemory::Zero(void *Block, CeU64 size)
    {
        Platform::MemSet(Block, 0, size);
    }

    void CeMemory::TracePrintSize(const char *message, CeU64 size)
    {
        const char *format;
        int value;

        const CeU64 gib = 1024 * 1024 * 1024;
        const CeU64 mib = 1024 * 1024;
        const CeU64 kib = 1024;

        if (size >= gib)
        {
            value = size / gib;
            format = "G";
        }
        else if (size >= mib)
        {
            value = size / mib;
            format = "M";
        }
        else if (size >= kib)
        {
            value = size / kib;
            format = "K";
        }
        else
        {
            value = size;
            format = "B";
        }

        CE_CORE_TRACE("%s%i %s", message, value, format);
    }
}