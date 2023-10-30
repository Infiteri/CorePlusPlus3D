#pragma once

#include "Base.h"

#include "Renderer/Objects/Mesh.h"
#include "Renderer/Camera/CameraSystem.h"
#include "Renderer/Light/PointLight.h"

namespace Core
{
    class Actor;
    class ActorScript;

    class CE_API Component
    {
    public:
        Actor *owner;

        bool custom = false;

        Component(){};
        ~Component(){};

        virtual void Init(){};
        virtual void Destroy(){};

        virtual void Start(){};
        virtual void Stop(){};
        virtual void Render(){};

        // Only used if is a custom component.
        virtual void OnImGuiRender(){};

        virtual void From(Component *other){};
    };

    class CE_API MeshComponent : public Component
    {
    public:
        Mesh *mesh;

        MeshComponent();
        ~MeshComponent();

        void Init();
        void Render();
        void Destroy();

        void From(MeshComponent *other);

        void SetGeometry(Geometry *geo);
        void SetMaterialFromName(const std::string &name);
    };

    class CE_API ActorScriptComponent : public Component
    {
    public:
        std::string className;

        void From(ActorScriptComponent *other);

        ActorScriptComponent();
        ~ActorScriptComponent();
    };

    class CE_API PerspectiveCameraComponent : public Component
    {
    public:
        PerspectiveCamera *camera;

        void From(PerspectiveCameraComponent *other);

        PerspectiveCameraComponent();
        ~PerspectiveCameraComponent();

        void Destroy();
        void Start();
        void Render();
    };

    class CE_API AABBComponent : public Component
    {
    public:
        float x, y, z, width, height, depth = 0;
        Vector2 padding;

        AABBComponent(){};
        ~AABBComponent(){};

        void Render();

        void From(AABBComponent *other);

        bool CollidesWith(AABBComponent *other);

        bool CollidesX(AABBComponent *other);
        bool CollidesY(AABBComponent *other);
    };

    class CE_API PointLightComponent : public Component
    {
    public:
        PointLight *light;

        PointLightComponent();
        ~PointLightComponent();

        void From(PointLightComponent *other);

        void Destroy();
        void Render();
    };
}