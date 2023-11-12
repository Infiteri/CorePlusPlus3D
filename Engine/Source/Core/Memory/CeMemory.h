#pragma once

//? ------------------------------ Welcome to Core/Memory/CeMemory.h --------------------------------
//? All the functions inside the CeMemory class are for use, it serves as a nice interface over the--
//? memory allocation / freeing / setting and other functions, also allows for memory size printing.-
//? -------------------------------------------------------------------------------------------------

#include "Base.h"

namespace Core
{
    /// @brief Class that is used for memory allocations and other + utils.
    class CE_API CeMemory
    {
    public:
        CeMemory(){};
        ~CeMemory(){};

        /// @brief Allocates memory.
        /// @param size The size of the memory, use "sizeof(...)" or a number.
        /// @return A void* to the newly allocated memory.
        static void *Allocate(CeU64 size);

        /// @brief Copies the memory from a source to a block.
        /// @param Block The block that the memory will be copied to.
        /// @param Source The block of memory to copy from.
        /// @param size The size of the memory, use "sizeof(...)" or a number.
        /// @return A void* to the newly copied memory.
        static void *Copy(void *Block, const void *Source, CeU64 size);

        /// @brief Sets the memory of the block to the value.
        /// @param Block The block of memory to set.
        /// @param value The value to set to, use 0 for empty.
        /// @param size The size of the memory, use "sizeof(...)" or a number.
        /// @return A void* to the newly set memory.
        static void *Set(void *Block, CeU32 value, CeU64 size);

        /// @brief Frees the block of memory.
        /// @param Block The block of memory to free.
        static void Free(void *Block);

        /// @brief Zeros a block of memory. Not same as free.
        /// @param Block The block to zero/
        /// @param size The size of the memory, use "sizeof(...)" or a number.
        static void Zero(void *Block, CeU64 size);

        /// @brief Logs a trace message of the size of a block of memory. The format is "%s%i %s", the passed in message, size of the memory formatted, "G"/"M"/"K"/"B".
        /// @param message The message to use.
        /// @param size The size of the memory, use "sizeof(...)" or a number.
        static void TracePrintSize(const char *message, CeU64 size);
    };

}