#include "EditorLayer.h"
#include "Core.h"

namespace Core
{
    void EditorLayer::OnAttach()
    {
                    Renderer::SetBackgroundColor(0, 0, 0.1 * 255, 255);
            MaterialManager::Load("EngineResources/Materials/Default.ce_mat");

            // World Setup
            World::Create("NewScene");
            World::Activate("NewScene");

            World::GetActive()->GenerateAndActivateSceneCamera("DefaultCamera", Math::DegToRad(90), Engine::GetWindowAspect(), 0.01f, 1000.0f);

            Actor *a = new Actor();
            World::GetActive()->AddActor(a);
            auto mesh = a->AddComponent<MeshComponent>();
            mesh->SetGeometry(new BoxGeometry(1, 1, 1));
            mesh->SetMaterialFromName("Material");
            a->AddComponent<ActorScriptComponent>()->className = "PlayerScript";
    }

    void EditorLayer::OnRender()
    {
    }

    void EditorLayer::OnUpdate()
    {
    }


    void EditorLayer::OnImGuiRender()
    {
    }

    void EditorLayer::OnDetach()
    {
    }
}