#pragma once

#include "Base.h"
#include "Color.h"

namespace Core
{
    class Shader;

    /// @brief Simple structure that defines a viewport.
    /// width = The viewport's width
    /// height = The viewport's height
    struct Viewport
    {
        int width = 1280;
        int height = 720;
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
        static Color *GetBackgroundColor();
        static void SetDepthMode(DepthMode mode);

        /// @brief Sets the backgrounds color. Could be useless if the sky mode isn't set to color.
        /// @param r The red channel. (0 - 255)
        /// @param g The green channel. (0 - 255)
        /// @param b The blue channel. (0 - 255)
        /// @param a The alpha channel. (0 - 255)
        static void SetBackgroundColor(float r, float g, float b, float a);
    };
}