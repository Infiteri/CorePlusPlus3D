#include "SceneHierarchyPanel.h"

#include "Core/Logger.h"

#include "Scene/World.h"
#include "Scene/Actor.h"

#include "Renderer/Geometry/BoxGeometry.h"
#include "Renderer/Geometry/PlaneGeometry.h"

#include "Core/Memory/CeMemory.h"

#include "CoreEditorUtils.h"

#include "Utils/StringUtils.h"

namespace Core
{
    static const ImGuiColorEditFlags colorEditFlags = ImGuiColorEditFlags_NoInputs;
    static char FileMaterialNameBuffer[256];

    SceneHierarchyPanel::SceneHierarchyPanel()
    {
        this->selectionContext = nullptr;
        this->scene = nullptr;

        CeMemory::Zero(&FileMaterialNameBuffer, 256);
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
        // Right-Click
        if (ImGui::BeginPopupContextWindow(0, 1))
        {
            if (ImGui::MenuItem("Add New Actor"))
                scene->AddActor(new Actor());

            ImGui::EndPopup();
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
            selectionContext = a;

        if (pop)
            ImGui::TreePop();
    }

    // ---------- UI Methods ------------------
    void DrawMeshUI(MeshComponent *m, Actor *a);
    void DrawActorScriptUI(ActorScriptComponent *scr, Actor *a);
    // ----------------------------------------

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
        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_AllowItemOverlap;
        bool transformOpen = ImGui::TreeNodeEx((void *)typeid(Transform).hash_code(), treeNodeFlags, "Transform");
        if (transformOpen)
        {
            EditorUtils::ImGuiVector3Edit("Position", a->GetTransform()->GetPosition(), 0.0f);
            EditorUtils::ImGuiVector3Edit("Rotation", a->GetTransform()->GetRotation(), 0.0f);
            EditorUtils::ImGuiVector3Edit("Scale", a->GetTransform()->GetScale(), 1.0f);
            ImGui::TreePop();
        }

        // UI
        EditorUtils::DrawComponentUI<MeshComponent>("Mesh", a, [&](MeshComponent *comp)
                                                    { DrawMeshUI(comp, a); });

        EditorUtils::DrawComponentUI<ActorScriptComponent>("Actor Script", a, [&](ActorScriptComponent *comp)
                                                           { DrawActorScriptUI(comp, a); });

        // Delete Entity
        if (ImGui::Button("Destroy"))
        {
            if (selectionContext && a->GetID() == selectionContext->GetID())
                selectionContext = nullptr;

            scene->RemoveActorByID(a->GetID());
        }

        ImGui::SameLine();

        if (ImGui::Button("Add Component"))
        {
            ImGui::OpenPopup("ComponentPopup");
        }

        if (ImGui::BeginPopupContextItem("ComponentPopup"))
        {
            if (ImGui::MenuItem("Mesh"))
            {
                selectionContext->AddComponent<MeshComponent>()->SetGeometry(new BoxGeometry(1, 1, 1));
            }

            if (ImGui::MenuItem("Script"))
            {
                selectionContext->AddComponent<ActorScriptComponent>();
            }

            ImGui::EndPopup();
        }
    }

    // ---------- UI Methods ------------------

    void DrawMeshUI(MeshComponent *m, Actor *a)
    {
        auto material = m->mesh->GetMaterial();
        float colors[4] = {material->GetColor()->r / 255, material->GetColor()->g / 255, material->GetColor()->b / 255, material->GetColor()->a / 255};

        // -- Material --
        if (m->mesh->IsMaterialUnique())
        {
            static char NameBuffer[256];
            CeMemory::Zero(&NameBuffer, 256);
            CeMemory::Copy(&NameBuffer, material->GetName().c_str(), 256);

            if (ImGui::InputText("Name", NameBuffer, 256))
            {
                material->SetName(NameBuffer);
            }

            Color *color = material->GetColor();
            if (ImGui::ColorEdit4("Color", colors, colorEditFlags))
            {
                Color *color = material->GetColor();
                color->Set4(colors, 255);
            }

            ImGui::Button("Texture");

            if (ImGui::BeginDragDropTarget())
            {
                const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CE_CONTENT_PANEL");

                if (payload)
                {
                    const char *name = (const char *)payload->Data;
                    std::string ext = StringUtils::GetFileExtension(name);
                    if (ext == "png" || ext == "jpg" || ext == "ce_image")
                    {
                        material->SetColorTexture(name);
                    }
                }

                ImGui::EndDragDropTarget();
            }

            ImGui::Separator();

            ImGui::InputText("File Name", FileMaterialNameBuffer, 256);

            if (ImGui::Button("Form File"))
            {
                m->mesh->SetMaterial(FileMaterialNameBuffer);
                CeMemory::Zero(&FileMaterialNameBuffer, 256);
            }

            if (ImGui::BeginDragDropTarget())
            {
                const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CE_CONTENT_PANEL");

                if (payload)
                {
                    const char *name = (const char *)payload->Data;
                    if (StringUtils::GetFileExtension(name).compare("   ce_mat") == 0)
                    {
                        m->mesh->SetMaterial(name);
                        CeMemory::Zero(&name, 256);
                    }
                }

                ImGui::EndDragDropTarget();
            }
        }
        else
        {
            ImGui::Text(material->GetName().c_str());

            if (ImGui::Button("Make Unique"))
            {
                m->mesh->MakeMaterialUnique();
            }

            if (ImGui::BeginDragDropTarget())
            {
                const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CE_CONTENT_PANEL");

                if (payload)
                {
                    const char *name = (const char *)payload->Data;
                    if (StringUtils::GetFileExtension(name).compare("ce_mat") == 0)
                    {
                        m->mesh->SetMaterial(name);
                        CeMemory::Zero(&name, 256);
                    }
                }

                ImGui::EndDragDropTarget();
            }
        }

        ImGui::Separator();

        // -- Geometry --

        const char *geometryTypeStrings[] = {"None", "Box", "Plane"};
        const char *geometryTypeCurrent = geometryTypeStrings[(int)m->mesh->GetGeometry()->GetType()];

        if (ImGui::BeginCombo("Geometry", geometryTypeCurrent))
        {
            for (int i = 0; i < 3; i++)
            {
                bool isSelected = (geometryTypeCurrent == geometryTypeStrings[i]);

                if (ImGui::Selectable(geometryTypeStrings[i], isSelected))
                {
                    geometryTypeCurrent = geometryTypeStrings[i];

                    if (geometryTypeCurrent == "Box")
                        m->mesh->SetGeometry(new BoxGeometry(1, 1, 1));
                    else if (geometryTypeCurrent == "Plane")
                        m->mesh->SetGeometry(new PlaneGeometry(1, 1));
                    else
                        m->mesh->SetGeometry(new Geometry());
                }

                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        // Edit geometry props
        Geometry *geometry = m->mesh->GetGeometry();
        if (geometry->GetType() == GeometryType::Box)
        {
            BoxGeometry *geo = (BoxGeometry *)geometry;

            float edit[3] = {geo->width, geo->height, geo->depth};

            if (ImGui::DragFloat3("Sizes", edit, 0.1, 0))
                m->mesh->SetGeometry(new BoxGeometry(edit[0], edit[1], edit[2]));
        }
        else if (geometry->GetType() == GeometryType::Plane)
        {
            PlaneGeometry *geo = (PlaneGeometry *)geometry;

            float edit[2] = {geo->width, geo->height};

            if (ImGui::DragFloat2("Sizes", edit, 0.1, 0.0))
                m->mesh->SetGeometry(new PlaneGeometry(edit[0], edit[1]));
        }
    }

    void DrawActorScriptUI(ActorScriptComponent *scr, Actor *a)
    {
        // Name
        char NameBuffer[256];
        CeMemory::Zero(&NameBuffer, 256);
        CeMemory::Copy(&NameBuffer, scr->className.c_str(), scr->className.size() <= 256 ? scr->className.size() : 256);

        // Edit name
        if (ImGui::InputText("Script Class Name", NameBuffer, 256))
            scr->className = NameBuffer;
    }

    // ----------------------------------------

}