#pragma once

#include "Base.h"

#include "Renderer/Objects/Mesh.h"

namespace Core
{
    class Actor;
    class ActorScript;

    class Component
    {
    public:
        Actor *owner;

        Component(){};
        ~Component(){};

        virtual void Init(){};
        virtual void Destroy(){};

        virtual void Start(){};
        virtual void Stop(){};
        virtual void Render(){};

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

}