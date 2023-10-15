#include "Components.h"

#include "Core/Logger.h"

#include "Scene/Actor.h"
#include "Script/ActorScript.h"

#include "Renderer/Geometry/BoxGeometry.h"
#include "Renderer/Geometry/PlaneGeometry.h"

namespace Core
{
    MeshComponent::MeshComponent()
    {
        mesh = new Mesh();
    }

    MeshComponent::~MeshComponent()
    {
    }

    void MeshComponent::Init()
    {
        mesh->Init();
    }

    void MeshComponent::Render()
    {
        mesh->Render();
    }

    void MeshComponent::Destroy()
    {
        mesh->Destroy();
        delete mesh;
    }

    void MeshComponent::From(MeshComponent *other)
    {
        Material *mat = other->mesh->GetMaterial();
        Geometry *geo = other->mesh->GetGeometry();

        // Material
        if (other->mesh->IsMaterialUnique())
        {
            mesh->GetMaterial()->GetColor()->Set(other->mesh->GetMaterial()->GetColor());

            if (mat->GetColorTexture()->HasImage())
                mesh->GetMaterial()->SetColorTexture(mat->GetColorTexture()->GetImagePath());
        }
        else
        {
            SetMaterialFromName(other->mesh->GetMaterial()->GetName());
        }

        // Geometry
        if (geo->GetType() == GeometryType::Box)
        {
            BoxGeometry *nGeo = (BoxGeometry *)geo;
            mesh->SetGeometry(new BoxGeometry(nGeo->width, nGeo->height, nGeo->depth));
        }
        else if (geo->GetType() == GeometryType::Plane)
        {
            PlaneGeometry *nGeo = (PlaneGeometry *)geo;
            mesh->SetGeometry(new PlaneGeometry(nGeo->width, nGeo->height));
        }
    }

    void MeshComponent::SetGeometry(Geometry *geo)
    {
        mesh->SetGeometry(geo);
    }

    void MeshComponent::SetMaterialFromName(const std::string &name)
    {
        mesh->SetMaterial(name);
    }

    void ActorScriptComponent::From(ActorScriptComponent *other)
    {
        className = other->className;
    }

    ActorScriptComponent::ActorScriptComponent()
    {
    }

    ActorScriptComponent::~ActorScriptComponent()
    {
    }
}