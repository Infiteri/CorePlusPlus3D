#include "FrameBuffer.h"
#include "Core/Logger.h"
#include "Core/Memory/CeMemory.h"

#include <glad/glad.h>

namespace Core
{
    GLenum ClearToGL(RenderPassTextureType type)
    {
        switch (type)
        {
        case RenderPassTextureType::Rgba8:
            return GL_RGBA8;
            break;

        case RenderPassTextureType::Depth:
            return GL_DEPTH24_STENCIL8;
            break;
        }

        CE_CORE_WARN("IDK");
        return GL_RGB;
    }

    FrameBuffer::FrameBuffer(FrameBufferSpecification spec)
    {
        // Set specification
        specification = spec;

        Create();
    }

    FrameBuffer::~FrameBuffer()
    {
        Destroy();

        CeMemory::Free(specification.renderPassSpecifications);
    }

    void FrameBuffer::Destroy()
    {
        DestroyForResize();
    }

    void FrameBuffer::DestroyForResize()
    {
        if (id != 0)
        {
            glDeleteFramebuffers(1, &id);
            id = 0; // Reset id after deletion
        }

        for (CeU32 i = 0; i < specification.renderPassSpecificationCount; i++)
        {
            RenderPassSpecification *pass = &specification.renderPassSpecifications[i];

            if (pass->textureType != RenderPassTextureType::Depth)
            {
                if (pass->id != 0)
                {
                    glDeleteTextures(1, &pass->id);
                    pass->id = 0; // Reset id after deletion
                }
            }
            else
            {
                if (pass->id != 0)
                {
                    glDeleteRenderbuffers(1, &pass->id);
                    pass->id = 0; // Reset id after deletion
                }
            }
        }
    }

    void FrameBuffer::Resize(CeU32 w, CeU32 h)
    {
        DestroyForResize();
        specification.width = w;
        specification.height = h;

        Create();
    }

    void FrameBuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, id);
    }

    void FrameBuffer::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::Create()
    {

        glGenFramebuffers(1, &id);
        glBindFramebuffer(GL_FRAMEBUFFER, id);

        // DONE: Create render passes
        for (CeU32 i = 0; i < specification.renderPassSpecificationCount; i++)
        {
            RenderPassSpecification *pass = &specification.renderPassSpecifications[i];

            if (pass->textureType != RenderPassTextureType::Depth)
            {
                glGenTextures(1, &pass->id);
                glBindTexture(GL_TEXTURE_2D, pass->id);

                // Colors go from 0 to 255, a unsigned byte or u8 is a char which holds 1 byte of memory, that goes from 0, to 255.
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, specification.width, specification.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glBindTexture(GL_TEXTURE_2D, 0);

                // Attach to framebuffer
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, pass->id, 0);
                pass->index = i;
            }
            else
            {
                glGenRenderbuffers(1, &pass->id);
                glBindRenderbuffer(GL_RENDERBUFFER, pass->id);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, specification.width, specification.height);
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, pass->id);
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, pass->id);
            }
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    RenderPassSpecification *FrameBuffer::GetRenderPass(CeU32 index)
    {
        if (index <= specification.renderPassSpecificationCount)
        {
            return &specification.renderPassSpecifications[index];
        }
        else
        {
            CE_CORE_ERROR("Trying to get a pass at invalid index.");
            return nullptr;
        }
    }

    CeU32 FrameBuffer::GetID()
    {
        return id;
    }
}