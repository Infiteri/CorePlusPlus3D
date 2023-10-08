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
        bool isMaterialUnique = false;
        Geometry *geometry = nullptr;

    public:
        Mesh();
        ~Mesh();

        void SetMaterial(const std::string &name);
        void SetGeometry(Geometry *_geometry);

        inline Material *GetMaterial() { return material; };
        inline Geometry *GetGeometry() { return geometry; };
        inline bool IsMaterialUnique() { return isMaterialUnique; };

        void Init();

        /// @brief Makes the material unique, is not ref counted and deleted automatically. If two materials are marked as unique, but always have the same props, thats the users fault.
        void MakeMaterialUnique();

        void Render();
        void Destroy();
    };
} // namespace  Core
