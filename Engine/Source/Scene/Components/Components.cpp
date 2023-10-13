#include "Components.h"

#include "Core/Logger.h"

#include "Scene/Actor.h"
#include "Script/ActorScript.h"

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

    void MeshComponent::SetGeometry(Geometry *geo)
    {
        mesh->SetGeometry(geo);
    }

    void MeshComponent::SetMaterialFromName(const std::string &name)
    {
        mesh->SetMaterial(name);
    }

    ActorScriptComponent::ActorScriptComponent()
    {
    }

    ActorScriptComponent::~ActorScriptComponent()
    {
    }
}