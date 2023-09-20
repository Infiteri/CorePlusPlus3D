#pragma once

#include "Base.h"
#include "Buffer.h"

namespace Core
{
    class VertexArray
    {
    private:
        CeU32 id;

        Buffer *vertexBuffer;
        Buffer *indexBuffer;

    public:
        VertexArray();
        ~VertexArray();

        void Destroy();

        void Bind();
        void Unbind();

        CeU32 GetID();

        Buffer *GenVertexBuffer(void *data, CeU64 dataSize);
        Buffer *GenIndexBuffer(void *data, CeU64 dataSize);

        Buffer *GetVertexBuffer();
        Buffer *GetIndexBuffer();
    };

}