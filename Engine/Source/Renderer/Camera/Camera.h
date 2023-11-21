#pragma once

#include "Base.h"
#include "Renderer/Objects/Object3D.h"

namespace Core
{
    class CE_API Camera : public Object3D
    {
    public:
        enum MatrixMode
        {
            TransformMatrix,

            /// @brief Requires SetViewMatrix to be called for a valid matrix output. Use "TransformMatrix" to use the transform object for the matrix.
            InputMatrix
        };

    protected:
        Matrix4 projection;
        Matrix4 view;
        MatrixMode matrixMode;

    public:
        Camera();
        virtual ~Camera();

        virtual void UpdateProjection() = 0;

        void SetViewMatrix(const Matrix4 &other);
        Matrix4 GetViewMatrix();

        inline Matrix4 *GetProjection() { return &projection; };
        inline MatrixMode GetMatrixMode() { return matrixMode; };
        void SetMatrixMode(MatrixMode mode);
    };
}