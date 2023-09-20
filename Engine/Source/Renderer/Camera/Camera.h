#pragma once

#include "Base.h"
#include "Renderer/Objects/Object3D.h"

namespace Core
{
    class CE_API Camera : public Object3D
    {
    protected:
        Matrix4 projection;

    public:
        Camera();
        ~Camera();

        virtual void UpdateProjection();

        Matrix4 GetViewMatrix();

        inline Matrix4 *GetProjection() { return &projection; };
    };
}