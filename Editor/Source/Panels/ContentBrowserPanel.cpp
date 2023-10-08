#include "ContentBrowserPanel.h"
#include "Core/Logger.h"

#include "Renderer/Texture/Texture.h"

#include "Platform/Platform.h"

#include "Utils/StringUtils.h"

namespace Core
{
    static std::string baseResourcesPath = "EngineResources"; // TODO: From project
    static std::string activePath = baseResourcesPath;
    static bool pressedFolder = false;

    Texture *iconTexture;
    Texture *folderTexture;

    ContentBrowserPanel::ContentBrowserPanel()
    {
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
    }
}