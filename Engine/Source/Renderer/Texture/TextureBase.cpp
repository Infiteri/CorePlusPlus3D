#include "TextureBase.h"

namespace Core
{
    TextureBase::TextureBase()
    {
        id = 0;
        generation = 0;
        image = nullptr;
    }

    TextureBase::~TextureBase()
    {
    }

    void TextureBase::Load()
    {
    }

    void TextureBase::Bind()
    {
    }

    void TextureBase::Unbind()
    {
    }

    void TextureBase::Destroy()
    {
    }

    void TextureBase::Use()
    {
    }

    bool TextureBase::HasImage()
    {
        return image != nullptr;
    }

    std::string TextureBase::GetImagePath()
    {
        if (!HasImage())
            return "";

        return image->GetPath().c_str();
    }

    CeU32 TextureBase::GetID()
    {
        return id;
    }

    CeU32 TextureBase::GetGeneration()
    {
        return generation;
    }
}