#pragma once

#include "Base.h"

namespace Core
{
    enum RenderPassTextureType
    {
        Rgba8,
        Depth,
    };
    

    struct RenderPassSpecification
    {
        RenderPassTextureType textureType;
        CeU32 index;
        CeU32 id;

        RenderPassSpecification() { textureType = RenderPassTextureType::Rgba8; };
        RenderPassSpecification(RenderPassTextureType type) { textureType = type; };
        ~RenderPassSpecification(){};
    };

    struct FrameBufferSpecification
    {
        CeU32 width;
        CeU32 height;

        CeU32 renderPassSpecificationCount;
        RenderPassSpecification *renderPassSpecifications;
        ~FrameBufferSpecification(){};
    };

    class CE_API FrameBuffer
    {
    private:
        CeU32 id;
        FrameBufferSpecification specification;

    public:
        FrameBuffer(FrameBufferSpecification spec);
        ~FrameBuffer();

        void Destroy();
        void DestroyForResize();
        void Resize(CeU32 w, CeU32 h);

        void Bind();
        void Unbind();
        void Create();

        RenderPassSpecification *GetRenderPass(CeU32 index);

        CeU32 GetID();
    };
}