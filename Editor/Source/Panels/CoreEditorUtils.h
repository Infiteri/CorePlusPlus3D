#pragma once

#include "Math/Vectors.h"

namespace Core
{
    namespace EditorUtils
    {
        void ImGuiVector3Edit(const char *label, Vector3 *vec, float valueDefault = 0.0f);
    }
}