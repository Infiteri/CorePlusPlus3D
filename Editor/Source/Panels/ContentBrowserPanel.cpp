#include "ContentBrowserPanel.h"
#include "Core/Logger.h"
#include "Core/Memory/CeMemory.h"
#include "Platform/Platform.h"
#include "Renderer/Texture/Texture.h"
#include "Platform/Platform.h"
#include "Utils/StringUtils.h"
#include "Renderer/Material.h"

#include <fstream>

namespace Core
{
    static std::string baseResourcesPath = "EngineResources"; // TODO: From project
    static std::string activePath = baseResourcesPath;
    static bool pressedFolder = false;
    static bool displayCreateCubeMap = false;
    static bool displayCreateMaterial = false;

    char Name[256];
    char Right[256];
    char Left[256];
    char Top[256];
    char Bottom[256];
    char Front[256];
    char Back[256];
    MaterialConfiguration MaterialConfig;

    Texture *iconTexture;
    Texture *folderTexture;

    ContentBrowserPanel::ContentBrowserPanel()
    {
        CeMemory::Zero(Name, 256);
        CeMemory::Zero(Right, 256);
        CeMemory::Zero(Left, 256);
        CeMemory::Zero(Top, 256);
        CeMemory::Zero(Bottom, 256);
        CeMemory::Zero(Front, 256);
        CeMemory::Zero(Back, 256);
    }

    ContentBrowserPanel::~ContentBrowserPanel()
    {
    }

    void ContentBrowserPanel::LoadAssets()
    {
        iconTexture = new Texture();
        iconTexture->Load("EngineResources/Images/Icons/icon.png");

        folderTexture = new Texture();
        folderTexture->Load("EngineResources/Images/Icons/folder.png");
    }

    void ContentBrowserPanel::OnImGuiRender()
    {
        ImGui::Begin("Content Browser");

        static float padding = 16.0f;
        static float thumbnailSize = 64.0f;
        float cellSize = thumbnailSize + padding;
        float panelWidth = ImGui::GetContentRegionAvail().x;
        int columnCount = (int)(panelWidth / cellSize);
        if (columnCount < 1)
            columnCount = 1;
        ImVec2 size{thumbnailSize, thumbnailSize}; // Might change based on preferences
        ImGui::Columns(columnCount, 0, false);

        if (activePath.compare(baseResourcesPath) != 0)
        {
            if (ImGui::Button("<-"))
            {
                activePath = baseResourcesPath;
            }
        }

        if (ImGui::BeginPopupContextWindow(0, 1))
        {
            if (ImGui::MenuItem("Create Cube Map"))
                displayCreateCubeMap = true;

            if (ImGui::MenuItem("Create Material"))
                displayCreateMaterial = true;

            ImGui::EndPopup();
        }

        int it = 0;
        for (DirectoryEntry entry : Platform::GetDirectoryEntries(activePath))
        {
            it++;

            ImGui::PushID(it);

            Texture *texture = entry.isFolder ? folderTexture : iconTexture;
            if (ImGui::ImageButton((void *)(CeU64)(CeU32)texture->GetID(), size))
            {
                if (entry.isFolder)
                {
                    pressedFolder = true;
                    activePath += "/" + entry.path;
                }
            }

            if (ImGui::BeginDragDropSource())
            {
                std::string path = entry.path;
                ImGui::SetDragDropPayload("CE_CONTENT_PANEL", path.c_str(), strlen(path.c_str()) + 1);
                ImGui::EndDragDropSource();
            }

            if (!entry.isFolder)
                ImGui::TextWrapped(StringUtils::RemoveFileExtension(entry.path).c_str());
            else
                ImGui::TextWrapped(entry.path.c_str());

            ImGui::NextColumn();

            ImGui::PopID();
        }

        ImGui::Columns(1);

        ImGui::End();

        // Popup's
        if (displayCreateCubeMap)
        {
            ImGui::Begin("Create Cubemap");

            ImGui::InputText("Name", Name, 256);
            ImGui::InputText("Right", Right, 256);
            ImGui::InputText("Left", Left, 256);
            ImGui::InputText("Top", Top, 256);
            ImGui::InputText("Bottom", Bottom, 256);
            ImGui::InputText("Front", Front, 256);
            ImGui::InputText("Back", Back, 256);

            if (ImGui::Button("Create"))
            {
                std::ofstream fout(activePath + std::string("/") + std::string(Name) + ".ce_cubemap");

                fout << "left =" << Left << "\n";
                fout << "right =" << Right << "\n";
                fout << "top =" << Top << "\n";
                fout << "bottom =" << Bottom << "\n";
                fout << "front =" << Front << "\n";
                fout << "back =" << Back << "\n";

                fout.close();

                CeMemory::Zero(Name, 256);
                CeMemory::Zero(Right, 256);
                CeMemory::Zero(Left, 256);
                CeMemory::Zero(Top, 256);
                CeMemory::Zero(Bottom, 256);
                CeMemory::Zero(Front, 256);
                CeMemory::Zero(Back, 256);

                displayCreateCubeMap = false;
            }

            ImGui::End();
        }

        if (displayCreateMaterial)
        {
            ImGui::Begin("Create Material");
            ImGui::InputText("Material Name", Name, 256);

            float colors[4] = {MaterialConfig.color.r / 255, MaterialConfig.color.g / 255, MaterialConfig.color.b / 255, MaterialConfig.color.a / 255};
            if (ImGui::ColorEdit4("Color", colors))
            {
                MaterialConfig.color.r = colors[0] * 255;
                MaterialConfig.color.g = colors[1] * 255;
                MaterialConfig.color.b = colors[2] * 255;
                MaterialConfig.color.a = colors[3] * 255;
            };

            ImGui::Button("Texture");

            ImGui::SameLine();

            if (ImGui::Button("Open Texture File"))
            {
                std::string val = Platform::OpenFileDialog("Texture (*.png *.jpg *.jpeg *.ce_image)\0*.png *.jpg *.jpeg *.ce_image\0");
                if (!val.empty())
                    MaterialConfig.colorTextureName = val;
            }

            if (ImGui::BeginDragDropTarget())
            {
                const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("CE_CONTENT_PANEL");

                if (payload)
                {
                    const char *name = (const char *)payload->Data;
                    std::string ext = StringUtils::GetFileExtension(name);
                    if (ext == "png" || ext == "jpg" || ext == "ce_image")
                    {
                        MaterialConfig.colorTextureName = name;
                    }
                }

                ImGui::EndDragDropTarget();
            }

            if (ImGui::Button("Create"))
            {
                std::string materialName = std::string(Name);
                std::ofstream materialFile(activePath + "/" + materialName + ".ce_mat");

                if (materialFile.is_open())
                {
                    materialFile << "version = 0.1\n";
                    materialFile << "color = " << MaterialConfig.color.r << " " << MaterialConfig.color.g << " " << MaterialConfig.color.b << " " << MaterialConfig.color.a << "\n";

                    if (!MaterialConfig.colorTextureName.empty())
                    {
                        materialFile << "colorTextureName = " << MaterialConfig.colorTextureName.c_str() << "\n";
                    }

                    materialFile.close();
                    MaterialConfig.color.Set(255, 255, 255, 255);
                    MaterialConfig.colorTextureName = "";
                }

                CeMemory::Zero(Name, 256);
                displayCreateMaterial = false;
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancel"))
            {
                CeMemory::Zero(Name, 256);
                displayCreateMaterial = false;
            }

            ImGui::End();
        }
    }
}