#pragma once

#include "Base.h"

#include <vector>

namespace Core
{
    enum class BufferType
    {
        Vertex,
        Index
    };

    struct BufferLayout
    {
        CeU32 location;
        CeU32 offset;
        CeU32 size;

        BufferLayout(CeU32 l, CeU32 o, CeU32 s)
        {
            location = l;
            offset = o;
            size = s;
        };
    };

    class Buffer
    {
    private:
        CeU32 id;
        BufferType bufferType;
        CeU64 size;
        CeU64 stride;
        std::vector<BufferLayout> layouts;

    public:
        Buffer(BufferType type);
        ~Buffer();

        void UploadData(void *data, CeU64 dataSize);

        void Destroy();
        void Bind();
        void Unbind();

        void Draw();

        void AddLayout(CeU32 l, CeU32 o, CeU32 s);

        CeU32 GetID();
        BufferType GetBufferType();
    };
}