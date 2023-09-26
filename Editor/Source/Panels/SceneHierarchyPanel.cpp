#include "SceneHierarchyPanel.h"

#include "Scene/World.h"
#include "Scene/Actor.h"

#include "Core/Memory/CeMemory.h"

#include "CoreEditorUtils.h"

namespace Core
{
    SceneHierarchyPanel::SceneHierarchyPanel()
    {
        this->selectionContext = nullptr;
        this->scene = nullptr;
    }

    SceneHierarchyPanel::~SceneHierarchyPanel()
    {
    }

    void SceneHierarchyPanel::UpdateContextToWorldActive()
    {
        scene = World::GetActive();
    }

    void SceneHierarchyPanel::OnImGuiRender()
    {
        if (!scene || scene == nullptr)
            return;

        // Actor looping
        ImGui::Begin("Scene Hierarchy");
        for (Actor *actor : scene->GetActors())
        {
            RenderActor(actor);
        }
        ImGui::End();

        ImGui::Begin("Proprieties");
        if (selectionContext)
            DrawActorComponents(selectionContext);
        ImGui::End();
    }

    void SceneHierarchyPanel::RenderActor(Actor *a)
    {
        // Flags setup
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_AllowItemOverlap;
        if (selectionContext != nullptr && selectionContext->GetID() == a->GetID())
            flags |= ImGuiTreeNodeFlags_Selected;
        bool pop = ImGui::TreeNodeEx((void *)(CeU64)(CeU32)a->GetID(), flags, a->GetName().c_str());

        if (ImGui::IsItemClicked())
        {
            selectionContext = a;
        }

        if (pop)
            ImGui::TreePop();
    }

    void SceneHierarchyPanel::DrawActorComponents(Actor *a)
    {
        // Name
        static char NameBuffer[256];
        CeMemory::Zero(&NameBuffer, 256);
        CeMemory::Copy(&NameBuffer, a->GetName().c_str(), a->GetName().size() <= 256 ? a->GetName().size() : 256);

        // Edit name
        if (ImGui::InputText("Name", NameBuffer, 256))
            a->SetName(NameBuffer);

        // Transform edit
        EditorUtils::ImGuiVector3Edit("Position", a->GetTransform()->GetPosition(), 0.0f);
        EditorUtils::ImGuiVector3Edit("Rotation", a->GetTransform()->GetRotation(), 0.0f);
        EditorUtils::ImGuiVector3Edit("Scale", a->GetTransform()->GetScale(), 1.0f);
    }
}