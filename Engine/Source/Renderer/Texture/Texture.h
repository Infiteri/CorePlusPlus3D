#pragma once

#include "Base.h"
#include "TextureBase.h"

#include <string>

namespace Core
{
    /// @brief Simple image texture.
    /// Image: The actual image data
    /// Texture: Id, Generation and uses the image
    class CE_API Texture : public TextureBase
    {
    private:
        bool isDefault;

    public:
        Texture();
        ~Texture();

        /// @brief If its already created, call destroy before loading again
        virtual void Load() override;

        /// @brief If its already created, call destroy before loading again
        void Load(const std::string &_filepath);

        /// @brief If its already created, call destroy before loading again
        void Load(const std::string &_filepath, TextureConfiguration config);

        void Bind();
        void Unbind();
        void Destroy();
        void Use();

        void Load(CeU8 *data, int channel, CeU32 w, CeU32 h);

        int GetWidth();
        int GetHeight();

        /// @brief Do not call unless you're the TextureManager. If this is marked as default and its not it will not be cleared in the material.
        void MarkAsDefault(bool flag);
        bool IsMarkedAsDefault();
    };
}