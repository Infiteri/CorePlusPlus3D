#include "Geometry.h"

namespace Core
{
    Geometry::Geometry()
    {
    }

    Geometry::~Geometry()
    {
    }

    Vertex3D *Geometry::GetVertices()
    {
        return vertices.data();
    }

    CeU32 *Geometry::GetIndices()
    {
        return indices.data();
    }
}