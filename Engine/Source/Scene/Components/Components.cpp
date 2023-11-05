#include "Components.h"
#include "Math/Math.h"

#include "Core/Logger.h"
#include "Core/Engine.h"

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

            if (mat->GetNormalTexture()->HasImage())
                mesh->GetMaterial()->SetNormalTexture(mat->GetNormalTexture()->GetImagePath());
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

    void PerspectiveCameraComponent::From(PerspectiveCameraComponent *other)
    {
        camera->SetFOV(other->camera->GetFOV());
        camera->SetNear(other->camera->GetNear());
        camera->SetFar(other->camera->GetFar());
        camera->UpdateProjection(Engine::GetWindowAspect());
    }

    PerspectiveCameraComponent::PerspectiveCameraComponent()
    {
        camera = new PerspectiveCamera();
    }

    PerspectiveCameraComponent::~PerspectiveCameraComponent()
    {
    }

    void PerspectiveCameraComponent::Destroy()
    {
        delete camera;
    }

    void PerspectiveCameraComponent::Start()
    {
        camera->Update();
    }

    void PerspectiveCameraComponent::Render()
    {
        camera->GetPosition()->Set(&owner->GetTransform()->position);
        camera->GetRotation()->Set(&owner->GetTransform()->rotation);
    }

    void AABBComponent::Render()
    {
        x = owner->GetTransform()->position.x;
        y = owner->GetTransform()->position.y;
        z = owner->GetTransform()->position.z;
    }

    void AABBComponent::From(AABBComponent *other)
    {
        width = other->width;
        height = other->height;
        depth = other->depth;
        padding.x = other->padding.x;
        padding.y = other->padding.y;
    }

    // TODO: IMPLEMENT
    bool AABBComponent::CollidesWith(AABBComponent *other)
    {
        if (!other)
        {
            CE_ERROR("AABBComponent::CollidesWith: Other is nullptr.");
            return false;
        }

        if (CollidesX(other) && CollidesY(other))
            return true;

        return false;
    }

    bool AABBComponent::CollidesX(AABBComponent *other)
    {
        if (!other)
        {
            CE_ERROR("AABBComponent::CollidesX: Other is nullptr.");
            return false;
        }

        float left1 = x - width - padding.x;
        float right1 = x + width + padding.x;

        float left2 = other->x - other->width - other->padding.x;
        float right2 = other->x + other->width + other->padding.x;

        bool horizontalOverlap = (left1 <= right2) && (right1 >= left2);
        return horizontalOverlap;
    }

    bool AABBComponent::CollidesY(AABBComponent *other)
    {
        if (!other)
        {
            CE_ERROR("AABBComponent::CollidesY: Other is nullptr.");
            return false;
        }

        float top1 = y - height - padding.y;
        float bottom1 = y + height + padding.y;
        float top2 = other->y - other->height - other->padding.y;
        float bottom2 = other->y + other->height + other->padding.y;
        bool verticalOverlap = (top1 <= bottom2) && (bottom1 >= top2);

        return verticalOverlap;
    }

    PointLightComponent::PointLightComponent()
    {
        light = new PointLight();
    }

    PointLightComponent::~PointLightComponent()
    {
        Destroy();
    }

    void PointLightComponent::From(PointLightComponent *other)
    {
        light->GetPosition()->Set(other->light->GetPosition());
        light->GetColor()->Set(other->light->GetColor());
        light->GetSpecular()->Set(other->light->GetSpecular());
        light->SetQuadratic(other->light->GetQuadratic());
        light->SetLinear(other->light->GetLinear());
        light->SetConstant(other->light->GetConstant());
        light->SetRadius(other->light->GetRadius());
        light->SetIntensity(other->light->GetIntensity());
    }

    void PointLightComponent::Destroy()
    {
        delete light;
    }

    void PointLightComponent::Render()
    {
        light->GetPosition()->Set(&owner->GetTransform()->position);
        light->Update();
    }
}