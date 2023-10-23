#include "ContentBrowserPanel.h"
#include "Core/Logger.h"
#include "Core/Memory/CeMemory.h"
#include "Platform/Platform.h"
#include "Renderer/Texture/Texture.h"
#include "Platform/Platform.h"
#include "Utils/StringUtils.h"
#include "Renderer/Material.h"
#include "CoreEditorUtils.h"
#include "Project/Project.h"

#define IM_TEXT_UTIL(a) ImGui::InputText(#a, a, 256)

namespace Core
{
    static std::string baseResourcesPath = ""; // TODO: From project
    static std::string activePath = baseResourcesPath;
    static bool pressedFolder = false;
    static bool displayCreateCubeMap = false;
    static bool displayCreateScript = false;
    static bool displayCreateMaterial = false;
    static bool displayCreateFolder = false;
    static float padding = 16.0f;
    static float thumbnailSize = 64.0f;
    static float cellSize = thumbnailSize + padding;

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
        ZeroCharBuffers();
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

        if (activePath.empty())
        {
            if (Project::GetConfig() != nullptr)
            {
                baseResourcesPath = Project::GetConfig()->assetPath;
                activePath = baseResourcesPath;
            }

            ImGui::End();

            return;
        }
        float panelWidth = ImGui::GetContentRegionAvail().x;
        int columnCount = (int)(panelWidth / cellSize);
        if (columnCount < 1)
            columnCount = 1;
        ImVec2 size{thumbnailSize, thumbnailSize}; // Might change based on preferences
        ImGui::Columns(columnCount, 0, false);

        if (activePath.compare(baseResourcesPath) != 0 && ImGui::Button("<-"))
            activePath = baseResourcesPath;

        if (ImGui::BeginPopupContextWindow(0, 1))
        {
            if (ImGui::MenuItem("Create Cube Map"))
            {
                CeMemory::Copy(&Name, "CubeMap", 256);
                displayCreateCubeMap = true;
            }
            if (ImGui::MenuItem("Create Script"))
            {
                CeMemory::Copy(&Name, "Script", 256);
                displayCreateScript = true;
            }
            if (ImGui::MenuItem("Create Material"))
            {
                CeMemory::Copy(&Name, "Material", 256);
                displayCreateMaterial = true;
            }

            if (ImGui::MenuItem("Create Folder"))
            {
                CeMemory::Copy(&Name, "Folder", 256);
                displayCreateFolder = true;
            }
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
                ImGui::TextWrapped(StringUtils::GetFileName(entry.path).c_str());
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

            IM_TEXT_UTIL(Name);
            IM_TEXT_UTIL(Right);
            IM_TEXT_UTIL(Left);
            IM_TEXT_UTIL(Top);
            IM_TEXT_UTIL(Bottom);
            IM_TEXT_UTIL(Front);
            IM_TEXT_UTIL(Back);

            if (ImGui::Button("Create"))
            {
                std::ofstream fout(activePath + std::string("/") + std::string(Name) + ".ce_cubemap");

                FOutFieldEqualValue("left", Left, fout);
                FOutFieldEqualValue("right", Right, fout);
                FOutFieldEqualValue("top", Top, fout);
                FOutFieldEqualValue("bottom", Bottom, fout);
                FOutFieldEqualValue("front", Front, fout);
                FOutFieldEqualValue("back", Back, fout);
                ZeroCharBuffers();
                fout.close();

                displayCreateCubeMap = false;
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancel"))
            {
                ZeroCharBuffers();
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
                MaterialConfig.color.Set4(colors, 255);
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
                        MaterialConfig.colorTextureName = name;
                }

                ImGui::EndDragDropTarget();
            }

            if (ImGui::Button("Create"))
            {
                MaterialConfig.name = Name;
                EditorUtils::MaterialToFile(activePath + "/" + std::string(Name) + ".ce_mat", &MaterialConfig);
                MaterialConfig.color.Set(255, 255, 255, 255);
                MaterialConfig.colorTextureName = "";
                ZeroCharBuffers();
                displayCreateMaterial = false;
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancel"))
            {
                ZeroCharBuffers();
                displayCreateMaterial = false;
            }

            ImGui::End();
        }

        if (displayCreateFolder)
        {
            ImGui::Begin("Create Folder");
            ImGui::InputText("Folder Name", Name, 256);

            if (ImGui::Button("Create"))
            {
                Platform::CreateFolder(activePath + "/" + Name);
                displayCreateFolder = false;
                ZeroCharBuffers();
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancel"))
            {
                ZeroCharBuffers();
                displayCreateFolder = false;
            }

            ImGui::End();
        }

        if (displayCreateScript)
        {
            ImGui::Begin("Create Folder");
            ImGui::InputText("Script Name Name", Name, 256);

            if (ImGui::Button("Create"))
            {
                std::string NewName = activePath + "/" + std::string(Name) + ".h";

                std::ofstream hFile(NewName.c_str());
                if (hFile.is_open())
                {
                    hFile << "#pragma once\n";
                    hFile << "\n";
                    hFile << "#include <Core.h>\n";
                    hFile << "\n";
                    hFile << "class " << Name << " : public Core::ActorScript {\n";
                    hFile << "public:\n";
                    hFile << "\n";
                    hFile << Name << "();\n";
                    hFile << "~" << Name << "();\n";
                    hFile << "\n";
                    hFile << "};\n";
                    hFile << "\n";
                    hFile << "\n";
                    hFile << "CE_DEFINE_ACTOR_SCRIPT(" << Name << ");\n";
                }
                else
                {
                    CE_ERROR("File handle not open: %s", Name);
                }

                std::string NewName2 = activePath + "/" + std::string(Name) + ".cpp";

                std::ofstream cFile(NewName2.c_str());
                if (cFile.is_open())
                {
                    cFile << "#include <" << Name << ".h>\n";
                    cFile << "\n";
                    cFile << Name << "::" << Name << "() {}\n";
                    cFile << Name << "::"
                          << "~" << Name << "() {}\n";

                    cFile << "\n";
                    cFile << "CE_IMPLEMENT_ACTOR_SCRIPT_DEFINE(" << Name << ");\n";
                }
                else
                {
                    CE_ERROR("File handle not open: %s", Name);
                }

                displayCreateScript = false;
                ZeroCharBuffers();
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancel"))
            {
                ZeroCharBuffers();
                displayCreateScript = false;
            }

            ImGui::End();
        }
    }

    void ContentBrowserPanel::ZeroCharBuffers()
    {
        CeMemory::Zero(Name, 256);
        CeMemory::Zero(Right, 256);
        CeMemory::Zero(Left, 256);
        CeMemory::Zero(Top, 256);
        CeMemory::Zero(Bottom, 256);
        CeMemory::Zero(Front, 256);
        CeMemory::Zero(Back, 256);
    }

    void ContentBrowserPanel::FOutFieldEqualValue(const char *filedName, const char *value, std::ofstream &out)
    {
        out << filedName << " = " << value << "\n";
    }
}