#include "Actor.h"

#include "Renderer/Objects/Mesh.h"
#include "Renderer/Geometry/BoxGeometry.h"

namespace Core
{
    Actor::Actor()
    {
        mesh = new Mesh();
        mesh->SetMaterialFromName("Material");
        mesh->SetGeometry(new BoxGeometry(1, 1, 1));
        mesh->Init();
    }

    Actor::~Actor()
    {
    }

    void Actor::Init()
    {
    }

    void Actor::Destroy()
    {
    }

    void Actor::Update()
    {
    }

    void Actor::Render()
    {
        mesh->Render();
    }

    void Actor::Start()
    {
    }

    void Actor::Stop()
    {
    }
}