#include "PlaneGeometry.h"
namespace Core
{
    PlaneGeometry::PlaneGeometry(float _width, float _height)
    {
        width = _width;
        height = _height;

        type = GeometryType::Plane;

        const float vertexCount = 4 * 4; // 4 verts per segment
        vertices.resize(vertexCount);
        const float indexCount = 6 * 4; // 6 indices per segment
        indices.resize(indexCount);

        const float min_x = -width;
        const float min_y = -height;
        const float max_x = width;
        const float max_y = height;
        const float min_uvx = 0.0f;
        const float min_uvy = 0.0f;
        const float max_uvx = 1;
        const float max_uvy = 1;
        const float max_z = 0.0;
        const float min_z = 0.0;

        // Configure
        vertices[(0 * 4) + 0].position.Set(min_x, min_y, max_z);
        vertices[(0 * 4) + 1].position.Set(max_x, max_y, max_z);
        vertices[(0 * 4) + 2].position.Set(min_x, max_y, max_z);
        vertices[(0 * 4) + 3].position.Set(max_x, min_y, max_z);
        vertices[(0 * 4) + 0].uv.Set(min_uvx, min_uvy);
        vertices[(0 * 4) + 1].uv.Set(max_uvx, max_uvy);
        vertices[(0 * 4) + 2].uv.Set(min_uvx, max_uvy);
        vertices[(0 * 4) + 3].uv.Set(max_uvx, min_uvy);

        // Back face
        vertices[(1 * 4) + 0].position.Set(max_x, min_y, min_z);
        vertices[(1 * 4) + 1].position.Set(min_x, max_y, min_z);
        vertices[(1 * 4) + 2].position.Set(max_x, max_y, min_z);
        vertices[(1 * 4) + 3].position.Set(min_x, min_y, min_z);
        vertices[(1 * 4) + 0].uv.Set(min_uvx, min_uvy);
        vertices[(1 * 4) + 1].uv.Set(max_uvx, max_uvy);
        vertices[(1 * 4) + 2].uv.Set(min_uvx, max_uvy);
        vertices[(1 * 4) + 3].uv.Set(max_uvx, min_uvy);

        // Left
        vertices[(2 * 4) + 0].position.Set(min_x, min_y, min_z);
        vertices[(2 * 4) + 1].position.Set(min_x, max_y, max_z);
        vertices[(2 * 4) + 2].position.Set(min_x, max_y, min_z);
        vertices[(2 * 4) + 3].position.Set(min_x, min_y, max_z);
        vertices[(2 * 4) + 0].uv.Set(min_uvx, min_uvy);
        vertices[(2 * 4) + 1].uv.Set(max_uvx, max_uvy);
        vertices[(2 * 4) + 2].uv.Set(min_uvx, max_uvy);
        vertices[(2 * 4) + 3].uv.Set(max_uvx, min_uvy);

        // Right face
        vertices[(3 * 4) + 0].position.Set(max_x, min_y, max_z);
        vertices[(3 * 4) + 1].position.Set(max_x, max_y, min_z);
        vertices[(3 * 4) + 2].position.Set(max_x, max_y, max_z);
        vertices[(3 * 4) + 3].position.Set(max_x, min_y, min_z);
        vertices[(3 * 4) + 0].uv.Set(min_uvx, min_uvy);
        vertices[(3 * 4) + 1].uv.Set(max_uvx, max_uvy);
        vertices[(3 * 4) + 2].uv.Set(min_uvx, max_uvy);
        vertices[(3 * 4) + 3].uv.Set(max_uvx, min_uvy);

        for (CeU32 i = 0; i < 4; ++i)
        {
            CeU32 v_offset = i * 4;
            CeU32 i_offset = i * 6;
            indices[i_offset + 0] = v_offset + 0;
            indices[i_offset + 1] = v_offset + 1;
            indices[i_offset + 2] = v_offset + 2;
            indices[i_offset + 3] = v_offset + 0;
            indices[i_offset + 4] = v_offset + 3;
            indices[i_offset + 5] = v_offset + 1;
        }
    }

    PlaneGeometry::~PlaneGeometry()
    {
    }
}