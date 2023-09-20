#pragma once

#include "Object3D.h"
#include "Renderer/Material.h"
#include "Renderer/Buffer/VertexArray.h"

#include "Renderer/Geometry/Geometry.h"

#include <string>

namespace Core
{
    class CE_API Mesh : public Object3D
    {
    private:
        VertexArray array;
        Material *material = nullptr;
        Geometry *geometry = nullptr;

    public:
        Mesh();
        ~Mesh();

        void SetMaterialFromName(const std::string &name);
        void SetGeometry(Geometry *_geometry);

        void Init();

        void Render();
        void Destroy();
    };
} // namespace  Core
