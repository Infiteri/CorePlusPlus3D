#include "Components.h"
#include "cmath"
#include "Math/Math.h"
#include "Core/Logger.h"
#include "Core/Engine.h"
#include "Scene/Actor.h"
#include "Script/ActorScript.h"
#include "Renderer/Geometry/BoxGeometry.h"
#include "Renderer/Geometry/PlaneGeometry.h"
#include "Renderer/Objects/Mesh.h"
#include "Renderer/Camera/CameraSystem.h"
#include "Renderer/Light/PointLight.h"
#include "Renderer/Light/SpotLight.h"
#include "Physics/PhysicsBody.h"

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
        camera->SetMatrixMode(Camera::InputMatrix);
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
    }
    void PerspectiveCameraComponent::Render()
    {
        if (owner->GetParent())
        {
            // NOTE: The camera's transform is NEEDED for the cubemap model position to work. In theory it could be extracted from the camera's view matrix at, 12, 13 and 14. (Matrix4 data index).
            // NOTE: Things might be done in the future about this. But for now this should be sufficient.
            Matrix4 *model = owner->GetWorldMatrix();
            camera->GetTransform()->position.x = model->data[12];
            camera->GetTransform()->position.y = model->data[13];
            camera->GetTransform()->position.z = model->data[14];
            camera->SetMatrixMode(Camera::InputMatrix);
            camera->SetViewMatrix(*model);
        }
        else
        {
            camera->SetMatrixMode(Camera::TransformMatrix);
            camera->GetTransform()->From(owner->GetTransform());
        }
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
            CE_CORE_ERROR("AABBComponent::CollidesWith: Other is nullptr.");
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
            CE_CORE_ERROR("AABBComponent::CollidesX: Other is nullptr.");
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
            CE_CORE_ERROR("AABBComponent::CollidesY: Other is nullptr.");
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
    }

    void PointLightComponent::From(PointLightComponent *other)
    {
        if (!other)
            return;

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
        light->GetPosition()->Set(owner->GetWorldMatrix()->data[12], owner->GetWorldMatrix()->data[13], owner->GetWorldMatrix()->data[14]);
        light->Update();
    }

    SpotLightComponent::SpotLightComponent()
    {
        light = new SpotLight();
    }

    SpotLightComponent::~SpotLightComponent()
    {
        Destroy();
    }

    void SpotLightComponent::From(SpotLightComponent *other)
    {
        if (!other)
            return;

        light->GetPosition()->Set(other->light->GetPosition());
        light->GetRotation()->Set(other->light->GetRotation());
        light->SetCutOff(other->light->GetCutOff());
        light->SetOuterCutOff(other->light->GetOuterCutOff());
        light->GetSpecular()->Set(other->light->GetSpecular());
        light->GetColor()->Set(other->light->GetColor());
        light->GetSpecular()->Set(other->light->GetSpecular());
        light->SetConstant(other->light->GetConstant());
        light->SetLinear(other->light->GetLinear());
        light->SetQuadratic(other->light->GetQuadratic());
    }

    void SpotLightComponent::Destroy()
    {
        delete light;
    }

    void SpotLightComponent::Render()
    {
        light->GetTransform()->From(owner->GetTransform());
        light->Update();
    }

    DataComponent::DataComponent()
    {
    }

    DataComponent::~DataComponent()
    {
        for (auto Set : Sets)
        {
            Set->ClearDataBasedOnCurrentType();
            delete Set;
        }

        Sets.clear();
    }

    void DataComponent::From(DataComponent *other)
    {
        for (auto Set : other->Sets)
        {
            auto nSet = new Data::Set(sizeof(Vector2), new Vector2(), Data::DataVec2, "Empty Set");
            nSet->From(Set);
            Sets.push_back(nSet);
        }
    }

    Data::Set *DataComponent::GetSetWithName(const std::string &name)
    {
        for (auto s : Sets)
        {
            if (s->Name == name)
                return s;
        }

        return nullptr;
    }

    PhysicsComponent::PhysicsComponent()
    {
        Configuration.Velocity = Vector3(1, 0, 0);
        Configuration.Acceleration = Vector3(5, 0, 0);
        Configuration.Mass = 1.0f;
        Configuration.Damping = 1.0f;
        Configuration.Gravity = 9.8f;
        Configuration.Owner = owner;
    }

    PhysicsComponent::~PhysicsComponent()
    {
    }

#define CE_HELPER_PHYS(C) Configuration.C = other->Configuration.C

    void PhysicsComponent::From(PhysicsComponent *other)
    {
        CE_HELPER_PHYS(Velocity);
        CE_HELPER_PHYS(Acceleration);
        CE_HELPER_PHYS(Mass);
        CE_HELPER_PHYS(Damping);
        CE_HELPER_PHYS(Gravity);
        CE_HELPER_PHYS(Owner);
        CE_HELPER_PHYS(Size);
    }
}