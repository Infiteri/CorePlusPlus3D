#pragma once

#include "Base.h"
#include "Math/Vectors.h"

#include <vector>

namespace Core
{
    struct Vertex3D
    {
        Vector3 position;
        Vector2 uv;
        Vector3 normal;
    };

    enum class GeometryType
    {
        None = 0,
        Box,
        Plane
    };

    class CE_API Geometry
    {
    protected:
        std::vector<Vertex3D> vertices;
        std::vector<CeU32> indices;
        GeometryType type;

    public:
        Geometry();
        ~Geometry();

        Vertex3D *GetVertices();
        inline int GetVerticesSize() { return vertices.size() * sizeof(Vertex3D); };
        inline GeometryType GetType() { return type; };

        CeU32 *GetIndices();
        inline int GetIndicesSize() { return indices.size() * sizeof(CeU32); };
    };
}