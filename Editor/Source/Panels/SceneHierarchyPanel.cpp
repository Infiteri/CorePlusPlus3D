#include "SceneHierarchyPanel.h"

#include "Core/Logger.h"
#include "Core/Input.h"
#include "EditorLayer.h"

#include "Scene/World.h"
#include "Scene/Actor.h"

#include "Math/Math.h"

#include "Renderer/Geometry/BoxGeometry.h"
#include "Renderer/Geometry/PlaneGeometry.h"

#include "Core/Memory/CeMemory.h"

#include "CoreEditorUtils.h"

#include "Utils/StringUtils.h"

#include <algorithm>

namespace Core
{
    static const ImGuiColorEditFlags colorEditFlags = ImGuiColorEditFlags_NoInputs;
    static char FileMaterialNameBuffer[256];
    static bool renderCameraPerspective = false;

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

        if (Input::GetKey(Keys::Escape))
            selectionContext = nullptr;

        actorIndex = 0;
        for (int i = 0; i < scene->GetActors().size(); i++)
        {
            actorIndex = i;
            Actor *ctx = scene->GetActors()[i];
            RenderActor(ctx, nullptr, false, 0);
        }

        // Right-Click
        if (ImGui::BeginPopupContextWindow(0, 1))
        {
            if (ImGui::MenuItem("Add New Actor"))
                scene->AddActor(new Actor());

            if (ImGui::MenuItem("Add Mesh Actor"))
            {
                Actor *a = new Actor();
                scene->AddActor(a);
                a->AddComponent<MeshComponent>();
            }

            if (ImGui::MenuItem("Add Camera Actor"))
            {
                Actor *a = new Actor();
                scene->AddActor(a);
                a->AddComponent<PerspectiveCameraComponent>();
            }

            if (ImGui::MenuItem("Add Point Light Actor"))
            {
                Actor *a = new Actor();
                scene->AddActor(a);
                a->AddComponent<PointLightComponent>();
            }

            ImGui::EndPopup();
        }

        ImGui::End();

        ImGui::Begin("Proprieties");

        if (selectionContext)
            DrawActorComponents(selectionContext);

        ImGui::End();
    }

    void SceneHierarchyPanel::RenderActor(Actor *a, Actor *parent, bool parentNodeOpen, CeU32 deep)
    {
        if (!a || a == nullptr)
            return;

        // Flags setup
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_AllowItemOverlap;
        if (selectionContext != nullptr)
            if (selectionContext->GetUUID()->Get() == a->GetUUID()->Get())
                flags |= ImGuiTreeNodeFlags_Selected;

        bool pop = false;

        if (!parent)
        {
            pop = ImGui::TreeNodeEx((void *)(CeU64)(CeU32)a->GetUUID()->Get(), flags, a->GetName().c_str());
            if (ImGui::IsItemClicked())
                selectionContext = a;
        }
        else
        {
            if (parentNodeOpen)
            {
                pop = ImGui::TreeNodeEx((void *)(CeU64)(CeU32)a->GetUUID()->Get(), flags, a->GetName().c_str());
                if (ImGui::IsItemClicked())
                    selectionContext = a;
            }
        }

        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
        {
            ImGui::SetDragDropPayload("CE_SCENE_HIERARCHY_ACTOR", a->GetUUID(), sizeof(UUID));
            ImGui::EndDragDropSource();
        }

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CE_SCENE_HIERARCHY_ACTOR"))
            {
                UUID *uid = (UUID *)payload->Data;
                CE_CORE_TRACE("%s to %ull", a->GetName().c_str(), uid->Get());

                // NOTE: Actor is target, data is name of the actor to move
                Actor *child = GetChildInHierarchy(parent, uid);

                if (child)
                {
                    //? Selection context will be set to nothing
                    selectionContext = nullptr;

                    RmChildInHierarchy(child->GetParent(), uid);
                    a->AddChild(child);
                }
            }

            ImGui::EndDragDropTarget();
        }

        // NOTE: This dummy gets rendered wheter or not its in the hierarchy or as a child of an actor.
        if (!parent)
        {
            ImGui::Dummy({ImGui::GetWindowWidth(), 5});
        }
        else
        {
            if (parentNodeOpen)
                ImGui::Dummy({ImGui::GetWindowWidth(), 5});
        }

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CE_SCENE_HIERARCHY_ACTOR"))
            {
                scene->MoveActorInHierarchy((UUID *)payload->Data, actorIndex);
            }

            ImGui::EndDragDropTarget();
        }

        for (Actor *child : a->GetChildren())
            RenderActor(child, a, pop, deep + 1);

        if (pop)
            ImGui::TreePop();
    }

    // ---------- UI Methods ------------------
    void DrawMeshUI(MeshComponent *m, Actor *a);
    void DrawActorScriptUI(ActorScriptComponent *scr, Actor *a);
    void DrawCameraComponentUI(PerspectiveCameraComponent *c, Actor *a);
    void DrawAABBComponent(AABBComponent *comp, Actor *a);
    void DrawPointLightComponent(PointLightComponent *comp, Actor *a);
    // ----------------------------------------

    void SceneHierarchyPanel::DrawActorComponents(Actor *a)
    {
        if (!a)
            return;

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
            EditorUtils::ImGuiVector3Edit("Position", &a->GetTransform()->position, 0.0f);
            EditorUtils::ImGuiVector3Edit("Rotation", &a->GetTransform()->rotation, 0.0f);
            EditorUtils::ImGuiVector3Edit("Scale", &a->GetTransform()->scale, 1.0f);
            ImGui::TreePop();
        }

        // UI
        EditorUtils::DrawComponentUI<MeshComponent>("Mesh", a, [&](MeshComponent *comp)
                                                    { DrawMeshUI(comp, a); });

        EditorUtils::DrawComponentUI<ActorScriptComponent>("Actor Script", a, [&](ActorScriptComponent *comp)
                                                           { DrawActorScriptUI(comp, a); });

        EditorUtils::DrawComponentUI<PerspectiveCameraComponent>("Camera", a, [&](PerspectiveCameraComponent *comp)
                                                                 { DrawCameraComponentUI(comp, a); });

        EditorUtils::DrawComponentUI<AABBComponent>("AABB", a, [&](AABBComponent *comp)
                                                    { DrawAABBComponent(comp, a); });

        EditorUtils::DrawComponentUI<PointLightComponent>("Point Light", a, [&](PointLightComponent *comp)
                                                          { DrawPointLightComponent(comp, a); });

        for (auto comp : selectionContext->GetComponents())
        {
            if (comp->custom)
                comp->OnImGuiRender();
        }

        // Delete Entity
        if (ImGui::Button("Destroy"))
        {
            if (selectionContext && a->GetUUID()->Get() == selectionContext->GetUUID()->Get())
                selectionContext = nullptr;

            RmChildInHierarchy(a->GetParent(), a->GetUUID());
        }

        ImGui::SameLine();

        if (ImGui::Button("Add Component"))
        {
            ImGui::OpenPopup("ComponentPopup");
        }

        if (ImGui::BeginPopupContextItem("ComponentPopup"))
        {
            if (ImGui::MenuItem("Mesh"))
                selectionContext->AddComponent<MeshComponent>()->SetGeometry(new BoxGeometry(1, 1, 1));

            if (ImGui::MenuItem("Script"))
                selectionContext->AddComponent<ActorScriptComponent>();

            if (ImGui::MenuItem("Camera"))
                selectionContext->AddComponent<PerspectiveCameraComponent>();

            if (ImGui::MenuItem("AABB"))
                selectionContext->AddComponent<AABBComponent>();

            if (ImGui::MenuItem("Point Light"))
                selectionContext->AddComponent<PointLightComponent>();

            ImGui::EndPopup();
        }
    }

    Actor *SceneHierarchyPanel::GetChildInHierarchy(Actor *parent, UUID *uid)
    {
        Actor *returnValue = nullptr;
        bool found = false;

        if (!parent)
        {
            // Do the search in the scene first
            bool foundInScene = false;

            for (Actor *a : scene->GetActors())
            {
                if (a->GetUUID()->Get() == uid->Get())
                {
                    foundInScene = true;
                    returnValue = Actor::From(a);
                    break;
                }
            }

            if (!foundInScene)
            {

                for (Actor *a : scene->GetActors())
                {
                    returnValue = GetChildInHierarchy(a, uid);
                    if (returnValue)
                        break; // Exit the loop early if a match is found.
                }
            }
        }
        else
        {
            for (Actor *a : parent->GetChildren())
            {
                if (a->GetUUID()->Get() == uid->Get())
                {
                    found = true;
                    returnValue = Actor::From(a);
                    CE_CORE_TRACE("%s got copyed.", returnValue->GetName().c_str());
                    break;
                }

                if (!found)
                    GetChildInHierarchy(a, uid);
            }
        }

        return returnValue;
    }

    void SceneHierarchyPanel::RmChildInHierarchy(Actor *parent, UUID *uid)
    {
        bool found = false;

        if (!parent)
        {
            scene->RemoveActorByUUID(uid);
            found = true;
        }
        else
        {
            for (Actor *a : parent->GetChildren())
            {
                if (a->GetUUID()->Get() == uid->Get())
                {
                    parent->RemoveChildUUID(uid);
                    found = true;
                }

                if (!found)
                    RmChildInHierarchy(a, uid);
            }
        }
    }

    // ---------- UI Methods ------------------

    void DrawMeshUI(MeshComponent *m, Actor *a)
    {
        if (ImGui::TreeNodeEx("Material"))
        {

            auto material = m->mesh->GetMaterial();

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

                float shiny = material->GetShininess();
                if (ImGui::DragFloat("Shininess", &shiny, 0.01f, 0.01f))
                {
                    material->SetShininess(shiny);
                }

                Color *color = material->GetColor();
                EditorUtils::ImGuiColor4Edit("Color", color);

                if (ImGui::TreeNodeEx("Color Texture"))
                {
                    EditorUtils::DrawMeshTextureUI(EditorUtils::TextureColor, material);

                    ImGui::TreePop();
                }

                if (ImGui::TreeNodeEx("Normal Texture"))
                {
                    EditorUtils::DrawMeshTextureUI(EditorUtils::TextureNormal, material);
                    ImGui::TreePop();
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
                        if (StringUtils::GetFileExtension(name).compare("ce_mat") == 0)
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

            ImGui::TreePop();
        }

        // -- Geometry --
        if (ImGui::TreeNodeEx("Geometry"))
        {

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

            ImGui::TreePop();
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

        if (ImGui::BeginDragDropTarget())
        {
            const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CE_CONTENT_PANEL");
            if (payload)
            {
                const char *name = (const char *)payload->Data;
                if (name)
                {
                    auto filename = StringUtils::GetFileName(name);
                    filename = StringUtils::RemoveFileExtension(filename);
                    if (!filename.empty())
                    {
                        scr->className = filename;
                    }
                }
            }

            ImGui::EndDragDropTarget();
        }
    }

    void DrawCameraComponentUI(PerspectiveCameraComponent *c, Actor *a)
    {
        // views
        float fov = Math::RadToDeg(c->camera->GetFOV());
        float near = c->camera->GetNear();
        float far = c->camera->GetFar();

        if (ImGui::DragFloat("Fov", &fov))
        {
            c->camera->SetFOV(Math::DegToRad(fov));
            c->camera->UpdateProjection();
        }

        if (ImGui::DragFloat("Near", &near))
        {
            c->camera->SetNear(near);
            c->camera->UpdateProjection();
        }

        if (ImGui::DragFloat("Far", &far))
        {
            c->camera->SetFar(far);
            c->camera->UpdateProjection();
        }

        if (ImGui::Checkbox("View", &renderCameraPerspective))
        {
            EditorLayer::Get()->HandleViewGameCamera(renderCameraPerspective);
        }
    }

    void DrawAABBComponent(AABBComponent *comp, Actor *a)
    {
        ImGui::DragFloat("Width", &comp->width);
        ImGui::DragFloat("Height", &comp->height);
        ImGui::DragFloat("Depth", &comp->depth);
        ImGui::DragFloat("Padding X", &comp->padding.x, 0.01, 0.1);
        ImGui::DragFloat("Padding Y", &comp->padding.y, 0.01, 0.1);
    }

    void DrawPointLightComponent(PointLightComponent *comp, Actor *a)
    {
        // TODO: Gizmo position.
        EditorUtils::ImGuiColor4Edit("Color", comp->light->GetColor());
        EditorUtils::ImGuiVec3Edit("Specular", comp->light->GetSpecular());

        float constant = comp->light->GetConstant();
        float linear = comp->light->GetLinear();
        float quadratic = comp->light->GetQuadratic();
        float radius = comp->light->GetRadius();
        float intensity = comp->light->GetIntensity();

        if (ImGui::DragFloat("Constant", &constant, 0.01f, 0.01f, 100.0f))
        {
            comp->light->SetConstant(constant);
        }

        if (ImGui::DragFloat("Linear", &linear, 0.01f, 0.01f, 100.0f))
        {
            comp->light->SetLinear(linear);
        }

        if (ImGui::DragFloat("Quadratic", &quadratic, 0.01f, 0.01f, 100.0f))
        {
            comp->light->SetQuadratic(quadratic);
        }

        if (ImGui::DragFloat("Radius", &radius, 0.01f, 0.01f, 100.0f))
        {
            comp->light->SetRadius(radius);
        }

        if (ImGui::DragFloat("Intensity", &intensity, 0.01f, 0.01f, 100.0f))
        {
            comp->light->SetIntensity(intensity);
        }
    }
    // ----------------------------------------
}