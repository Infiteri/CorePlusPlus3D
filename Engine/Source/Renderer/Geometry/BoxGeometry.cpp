#include "BoxGeometry.h"

namespace Core
{
    BoxGeometry::BoxGeometry(float _width, float _height, float _depth)
    {
        width = _width;
        height = _height;
        depth = _depth;

        float vertexCount = 4 * 6; // 4 verts per segment
        vertices.resize(vertexCount);
        float indexCount = 6 * 6; // 6 indices per segment
        indices.resize(indexCount);

        float min_x = -width;
        float min_y = -height;
        float min_z = -depth;
        float max_x = width;
        float max_y = height;
        float max_z = depth;
        float min_uvx = 0.0f;
        float min_uvy = 0.0f;
        float max_uvx = 1;
        float max_uvy = 1;

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

        // Bottom face
        vertices[(4 * 4) + 0].position.Set(max_x, min_y, max_z);
        vertices[(4 * 4) + 1].position.Set(min_x, min_y, min_z);
        vertices[(4 * 4) + 2].position.Set(max_x, min_y, min_z);
        vertices[(4 * 4) + 3].position.Set(min_x, min_y, max_z);
        vertices[(4 * 4) + 0].uv.Set(min_uvx, min_uvy);
        vertices[(4 * 4) + 1].uv.Set(max_uvx, max_uvy);
        vertices[(4 * 4) + 2].uv.Set(min_uvx, max_uvy);
        vertices[(4 * 4) + 3].uv.Set(max_uvx, min_uvy);

        // Top face
        vertices[(5 * 4) + 0].position.Set(min_x, max_y, max_z);
        vertices[(5 * 4) + 1].position.Set(max_x, max_y, min_z);
        vertices[(5 * 4) + 2].position.Set(min_x, max_y, min_z);
        vertices[(5 * 4) + 3].position.Set(max_x, max_y, max_z);
        vertices[(5 * 4) + 0].uv.Set(min_uvx, min_uvy);
        vertices[(5 * 4) + 1].uv.Set(max_uvx, max_uvy);
        vertices[(5 * 4) + 2].uv.Set(min_uvx, max_uvy);
        vertices[(5 * 4) + 3].uv.Set(max_uvx, min_uvy);

        for (CeU32 i = 0; i < 6; ++i)
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

    BoxGeometry::~BoxGeometry()
    {
    }
}