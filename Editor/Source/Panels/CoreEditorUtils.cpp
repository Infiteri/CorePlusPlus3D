#include "CoreEditorUtils.h"
#include "Utils/StringUtils.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <fstream>
#include <ostream>

#define CE_SWITCH_SHADER_UITL(a) \
    case SkyShaderDataType::a:   \
        return #a;               \
        break;

#define CE_SWITCH_SHADER_UITL_STRING(a, b) \
    if (a == #b)                           \
    return SkyShaderDataType::b

namespace Core
{
    static Texture *PlaceHolderTexture;
    static const CeU32 size = 50;

    namespace EditorUtils
    {
        void InitAssets()
        {
            CeU8 *data;

            data = new CeU8[size * size * 4];
            for (CeU32 y = 0; y < size; ++y)
            {
                for (CeU32 x = 0; x < size; ++x)
                {
                    CeU8 color = ((x / 5) + (y / 5)) % 2 == 0 ? 255 : 200; // Alternate between white (255) and light gray (200)
                    CeU32 pixelOffset = (y * size + x) * 4;
                    data[pixelOffset] = color;     // Red
                    data[pixelOffset + 1] = color; // Green
                    data[pixelOffset + 2] = color; // Blue
                    data[pixelOffset + 3] = 255;   // Alpha (fully opaque)
                }
            }
            PlaceHolderTexture = new Texture();
            PlaceHolderTexture->Load(data, 4, size, size);

            delete[] data;
        };

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
                CE_CORE_ERROR("File handle not open: %s", name);
            }
        }

        void ImGuiColor4Edit(const char *label, Color *color)
        {
            float edit[4] = {color->r / 255, color->g / 255, color->b / 255, color->a / 255};

            if (ImGui::ColorEdit4(label, edit))
                color->Set4(edit, 255.0f);
        }

        void ImGuiVec2Edit(const char *label, Vector2 *vec)
        {
            float edit[2] = {vec->x, vec->y};

            if (ImGui::DragFloat2(label, edit, 0.01f))
                vec->Set(edit[0], edit[1]);
        }

        void ImGuiVec3Edit(const char *label, Vector3 *vec)
        {
            float edit[3] = {vec->x, vec->y, vec->z};

            if (ImGui::DragFloat3(label, edit, 0.01f))
                vec->Set(edit[0], edit[1], edit[2]);
        }

        void ImGuiVec4Edit(const char *label, Vector4 *vec)
        {
            float edit[4] = {vec->x, vec->y, vec->z, vec->w};

            if (ImGui::DragFloat4(label, edit, 0.01f))
                vec->Set(edit[0], edit[1], edit[2], edit[3]);
        }

        std::string ShaderDataTypeToString(SkyShaderDataType d)
        {
            switch (d)
            {
                CE_SWITCH_SHADER_UITL(Vec2);
                CE_SWITCH_SHADER_UITL(Vec3);
                CE_SWITCH_SHADER_UITL(Vec4);
                CE_SWITCH_SHADER_UITL(Color);

            case SkyShaderDataType::None:
            default:
                return "None";
                break;
            }
        }

        SkyShaderDataType StringToShaderDatatType(const std::string &val)
        {
            if (val == "None")
                return SkyShaderDataType::None;

            CE_SWITCH_SHADER_UITL_STRING(val, Vec2);
            CE_SWITCH_SHADER_UITL_STRING(val, Vec3);
            CE_SWITCH_SHADER_UITL_STRING(val, Vec4);
            CE_SWITCH_SHADER_UITL_STRING(val, Color);

            return SkyShaderDataType::None;
        }

        void DrawMeshTextureUI(TexType type, Core::Material *material)
        {

            Core::Texture *texture;

            switch (type)
            {
            case TextureColor:
                texture = material->GetColorTexture();
                break;

            case TextureNormal:
                texture = material->GetNormalTexture();
                break;
            }

            if (texture->HasImage())
            {
                float imgW = texture->GetWidth();
                float imgH = texture->GetHeight();
                ImGui::Text("Image: %s.", texture->GetImagePath().c_str());
                ImGui::Text("Size: %f width by %f height.", imgW, imgH);

                // NOTE: Should be last for the drag-drop feature.
                float div = 5.0f;
                ImGui::Image((void *)(CeU64)(CeU32)texture->GetID(), {imgW / div, imgH / div});
            }
            else
            {
                ImGui::Image((void *)(CeU64)(CeU32)PlaceHolderTexture->GetID(), {size, size});
            }

            if (ImGui::BeginDragDropTarget())
            {
                const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CE_CONTENT_PANEL");

                if (payload)
                {
                    const char *name = (const char *)payload->Data;
                    if (StringUtils::FileExtIsImage(name))
                    {
                        switch (type)
                        {
                        case TextureColor:
                            material->SetColorTexture(name);
                            break;

                        case TextureNormal:
                            material->SetNormalTexture(name);
                            break;
                        }
                    }
                }

                ImGui::EndDragDropTarget();
            }

            if (ImGui::Button("Remove"))
            {
                switch (type)
                {
                case TextureColor:
                    material->EmptyColorTexture();
                    break;

                case TextureNormal:
                    material->EmptyNormalTexture();
                    break;
                }
            }
        }

        void RenderMatrix4GuiText(float *data)
        {
            ImGui::Columns(4, "MatrixColumns", true);
            for (int i = 0; i < 16; ++i)
            {
                ImGui::Text("%.2f", data[i]);
                ImGui::NextColumn();
            }
            ImGui::Columns(1); // Reset columns to 1 at the end
        }

    }
}