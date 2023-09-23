#include "Mesh.h"

#include "Core/Logger.h"

#include "Renderer/Manager/MaterialManager.h"
#include "Renderer/Geometry/BoxGeometry.h"
#include "Renderer/ShaderSystem.h"

namespace Core
{
    Mesh::Mesh()
    {
        material = MaterialManager::GetDefault();
        transform = Transform();
        init = false;
    }

    Mesh::~Mesh()
    {
    }

    void Mesh::SetMaterialFromName(const std::string &name)
    {
        if (material && material->GetName().compare("DefaultMaterial") != 0)
            MaterialManager::Release(material->GetName());

        material = MaterialManager::Get(name);
    }

    void Mesh::SetGeometry(Geometry *_geometry)
    {
        if (geometry)
            delete geometry;

        // Set new geometry
        geometry = _geometry;

        if (init)
        {
            init = false;

            Init();
        }
    }

    void Mesh::Init()
    {
        if (init)
            return;

        init = true;

        // No geometry = no array
        if (geometry)
        {
            array.GenVertexBuffer(geometry->GetVertices(), geometry->GetVerticesSize());
            array.GenIndexBuffer(geometry->GetIndices(), geometry->GetIndicesSize());
            array.GetVertexBuffer()->AddLayout(0, 0, 3);
            array.GetVertexBuffer()->AddLayout(1, 3, 2);
            array.GetVertexBuffer()->AddLayout(2, 5, 3);
        }
    }

    void Mesh::Render()
    {
        // If no geometry then there is nothing to render
        if (!init || !geometry)
            return;

        Shader *shd = ShaderSystem::Get("EngineResources/Shaders/Object");
        shd->Mat4(GetTransformMatrix(), "uTransform");

        material->Use();

        array.Bind();
        array.GetVertexBuffer()->Bind();
        array.GetIndexBuffer()->Draw();
    }

    void Mesh::Destroy()
    {
        if (material && material->GetName().compare("DefaultMaterial") != 0)
            MaterialManager::Release(material->GetName());

        if (geometry)
            delete geometry;

        init = false;
    }
}