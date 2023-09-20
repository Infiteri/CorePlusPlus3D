#pragma once

#include "Base.h"

namespace Core
{
    class CE_API CeMemory
    {
    public:
        CeMemory(){};
        ~CeMemory(){};

        static void *Allocate(CeU64 size);
        static void *Copy(void *Block, const void *Source, CeU64 size);
        static void *Set(void *Block, CeU32 value, CeU64 size);

        static void Free(void *Block);
        static void Zero(void *Block, CeU64 size);
        static void TracePrintSize(const char *message, CeU64 size);
    };

}