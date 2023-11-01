#pragma once

#include "Base.h"
#include "Color.h"
#include "Sky/Sky.h"

namespace Core
{
    class FrameBuffer;
    class Shader;

    /// @brief Simple structure that defines a viewport.
    /// width = The viewport's width
    /// height = The viewport's height
    struct Viewport
    {
        int width = 1280;
        int height = 720;
    };

    enum class RenderMode
    {
        Full,
        Color,
        Texture,
        Normal,
        Light
    };

    /// @brief Enumeration that represents the common depth modes.
    enum class DepthMode
    {
        Less,

        /// @brief Best for expected results.
        Lequal,

        Always,
    };

    class CE_API Renderer
    {
    public:
        Renderer(){};
        ~Renderer(){};

        static void Init();
        static void Shutdown();

        static void BeginFrame();
        static void Render();
        static void EndFrame();

        /// @brief Called after the viewport is set.
        static void RegenerateObjectsWithNewViewport();
        static void DrawImageToScreen();
        static void Resize(CeU64 w, CeU64 h);

        static Shader *GetObjectShader();

        static DepthMode GetDepthMode();
        static void SetDepthMode(DepthMode mode);

        static RenderMode GetRenderMode();
        static void SetRenderMode(RenderMode mode);

        static FrameBuffer *GetFrameBuffer();
    };
}