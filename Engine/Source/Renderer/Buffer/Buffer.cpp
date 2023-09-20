#include "Buffer.h"
#include "Core/Logger.h"

#include <glad/glad.h>

namespace Core
{
    Buffer::Buffer(BufferType type)
    {
        //? Set Details
        id = 0;
        bufferType = type;
        size = 0;
        stride = 0;

        glGenBuffers(1, &id);
        Bind();
    }

    Buffer::~Buffer()
    {
        Destroy();
    }

    void Buffer::UploadData(void *data, CeU64 dataSize)
    {
        Bind();

        size = dataSize;

        switch (bufferType)
        {
        case BufferType::Vertex:
            glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
            break;

        case BufferType::Index:
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
            break;
        }
    }

    void Buffer::Destroy()
    {
        glDeleteBuffers(1, &id);
    }

    void Buffer::Bind()
    {
        switch (bufferType)
        {
        case BufferType::Vertex:
            glBindBuffer(GL_ARRAY_BUFFER, id);

            // Bind layouts
            for (CeU32 i = 0; i < layouts.size(); i++)
            {
                CeU32 l = layouts[i].location;
                CeU32 o = layouts[i].offset;
                CeU32 s = layouts[i].size;
                glEnableVertexAttribArray(l);
                glVertexAttribPointer(l, s, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void *)(o * sizeof(float)));
            }

            break;

        case BufferType::Index:
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
            break;
        }
    }

    void Buffer::Unbind()
    {
        switch (bufferType)
        {
        case BufferType::Vertex:
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            break;

        case BufferType::Index:
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            break;
        }
    }

    void Buffer::Draw()
    {
        switch (bufferType)
        {
        case BufferType::Vertex:
            glDrawArrays(GL_TRIANGLES, 0, size / stride);
            break;

        case BufferType::Index:
            glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, nullptr);
            break;
        }
    }

    void Buffer::AddLayout(CeU32 l, CeU32 o, CeU32 s)
    {
        if (bufferType == BufferType::Vertex)
        {
            stride += s;
            layouts.push_back({l, o, s});
        }
    }

    CeU32 Buffer::GetID()
    {
        return id;
    }

    BufferType Buffer::GetBufferType()
    {
        return bufferType;
    }
}