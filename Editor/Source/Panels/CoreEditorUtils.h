#pragma once

#include "Math/Vectors.h"

#include "Core.h"

#include <imgui.h>

#include <string>

namespace Core
{
    namespace EditorUtils
    {
        void ImGuiVector3Edit(const char *label, Vector3 *vec, float valueDefault = 0.0f);
        template <typename T, typename UIFun>
        void DrawComponentUI(const std::string &name, Actor *a, UIFun fun)
        {
            const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_AllowItemOverlap;

            if (a->HasComponent<T>())
            {
                auto comp = a->GetComponent<T>();

                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {4, 4});
                bool open = ImGui::TreeNodeEx((void *)typeid(T).hash_code(), flags, name.c_str());
                bool remove = false;
                bool removeComp = false;

                ImGui::PopStyleVar();

                if (open)
                {
                    fun(comp);
                    if (ImGui::Button("Remove"))
                    {
                        removeComp = true;
                    };
                    ImGui::TreePop();
                }

                if (removeComp)
                    a->RemoveComponent<T>();
            }
        }

        std::string GeometryTypeToString(GeometryType t);
        GeometryType StringToGeometryType(const std::string &v);
        void MaterialToFile(const std::string &name, MaterialConfiguration *mat);

        void ImGuiColor4Edit(const char *label, Color *color);
        void ImGuiVec2Edit(const char *label, Vector2 *vec);
        void ImGuiVec3Edit(const char *label, Vector3 *vec);
        void ImGuiVec4Edit(const char *label, Vector4 *vec);

        std::string ShaderDataTypeToString(SkyShaderDataType d);
        SkyShaderDataType StringToShaderDatatType(const std::string &val);
    }
}