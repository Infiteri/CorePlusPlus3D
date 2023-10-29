#include "CoreEditorUtils.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <fstream>
#include <ostream>

namespace Core
{
    namespace EditorUtils
    {
        void ImGuiVector3Edit(const char *label, Vector3 *vec, float valueDefault)
        {
            ImGui::PushID(label);

            ImGui::Columns(2);

            ImGui::SetColumnWidth(0, 100.0f);
            ImGui::Text(label);
            ImGui::NextColumn();
            ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});

            float lineH = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
            ImVec2 buttonSize = {lineH + 3.0f, lineH};

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
            if (ImGui::Button("X", buttonSize))
            {
                vec->x = valueDefault;
            }
            ImGui::PopStyleColor(3);

            ImGui::SameLine();
            ImGui::DragFloat("##x", &vec->x, 0.01f);
            ImGui::PopItemWidth();
            ImGui::SameLine();

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
            if (ImGui::Button("Y", buttonSize))
            {
                vec->y = valueDefault;
            }
            ImGui::PopStyleColor(3);

            ImGui::SameLine();
            ImGui::DragFloat("##y", &vec->y, 0.01f);
            ImGui::PopItemWidth();
            ImGui::SameLine();

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
            if (ImGui::Button("Z", buttonSize))
            {
                vec->z = valueDefault;
            }
            ImGui::PopStyleColor(3);

            ImGui::SameLine();
            ImGui::DragFloat("##z", &vec->z, 0.01f);
            ImGui::PopItemWidth();
            ImGui::PopStyleVar();

            ImGui::Columns(1);

            ImGui::PopID();
        }

        std::string GeometryTypeToString(GeometryType t)
        {
            switch (t)
            {
            case GeometryType::Box:
                return "Box";
                break;

            case GeometryType::Plane:
                return "Plane";
                break;

            default:
            case GeometryType::None:
                return "None";
                break;
            }

            return "None";
        }

        GeometryType StringToGeometryType(const std::string &v)
        {
            if (v == "None")
                return GeometryType::None;
            else if (v == "Box")
                return GeometryType::Box;
            else if (v == "Plane")
                return GeometryType::Plane;
            else
                return GeometryType::None;
        }

        void MaterialToFile(const std::string &name, MaterialConfiguration *mat)
        {
            std::ofstream materialFile(name);
            if (materialFile.is_open())
            {
                materialFile << "version = 0.1\n";
                materialFile << "color = " << mat->color.r << " " << mat->color.g << " " << mat->color.b << " " << mat->color.a << "\n";

                if (!mat->colorTextureName.empty())
                    materialFile << "colorTextureName = " << mat->colorTextureName.c_str() << "\n";
                else
                    materialFile << "colorTextureName = "
                                 << ""
                                 << "\n";
            }
            else
            {
                CE_ERROR("File handle not open: %s", name);
            }
        }

        void ImGuiColor4Edit(const char *label, Color *color)
        {
            float edit[4] = {color->r / 255, color->g / 255, color->b / 255, color->a / 255};

            if (ImGui::ColorEdit4(label, edit))
                color->Set4(edit, 255.0f);
        }

        void ImGuiVec3Edit(const char *label, Vector3 *vec)
        {
            float edit[3] = {vec->x, vec->y, vec->z};

            if (ImGui::DragFloat3(label, edit, 0.05f))
                vec->Set(edit[0], edit[1], edit[2]);
        }
    }
}