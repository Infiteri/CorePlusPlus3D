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
    };

    class Geometry
    {
    protected:
        std::vector<Vertex3D> vertices;
        std::vector<CeU32> indices;

    public:
        Geometry();
        ~Geometry();

        Vertex3D *GetVertices();
        inline int GetVerticesSize() { return vertices.size() * sizeof(Vertex3D); };

        CeU32 *GetIndices();
        inline int GetIndicesSize() { return indices.size() * sizeof(CeU32); };
    };
}