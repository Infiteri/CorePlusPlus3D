#include "VertexArray.h"

#include <glad/glad.h>

namespace Core
{
    VertexArray::VertexArray()
    {
        glGenVertexArrays(1, &id);
        glBindVertexArray(id);
     }

    VertexArray::~VertexArray()
    {
        Destroy();
    }

    void VertexArray::Destroy()
    {
        glDeleteVertexArrays(1, &id);
    }

    void VertexArray::Bind()
    {
        glBindVertexArray(id);
    }

    void VertexArray::Unbind()
    {
        glBindVertexArray(0);
    }

    CeU32 VertexArray::GetID()
    {
        return id;
    }

    Buffer *VertexArray::GenVertexBuffer(void *data, CeU64 dataSize)
    {
        vertexBuffer = new Buffer(BufferType::Vertex);
        vertexBuffer->UploadData(data, dataSize);

        return vertexBuffer;
    }

    Buffer *VertexArray::GenIndexBuffer(void *data, CeU64 dataSize)
    {

        indexBuffer = new Buffer(BufferType::Index);
        indexBuffer->UploadData(data, dataSize);

        return indexBuffer;
    }

    Buffer *VertexArray::GetVertexBuffer()
    {
        return vertexBuffer;
    }

    Buffer *VertexArray::GetIndexBuffer()
    {
        return indexBuffer;
    }
}