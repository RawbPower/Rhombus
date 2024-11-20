#include "rbpch.h"

#include "ContentBrowerPanel.h"

#include "Rhombus/Core/Application.h"
#include "Rhombus/Project/Project.h"
#include "Rhombus/Math/MAth.h"

#include <imgui/imgui.h>

namespace rhombus
{
	ContentBrowserPanel::ContentBrowserPanel()
		: m_baseDirectory(Project::GetAssetDirectory()), m_currentDirectory(m_baseDirectory)
	{
		m_FolderIcon = Texture2D::Create(Application::Get().GetPathRelativeToEditorDirectory("resources/icons/ContentBrowser/Folder.png"));
		m_FileIcon = Texture2D::Create(Application::Get().GetPathRelativeToEditorDirectory("resources/icons/ContentBrowser/File.png"));
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (m_currentDirectory != std::filesystem::path(m_baseDirectory))
		{
			if (ImGui::Button("../"))
			{
				m_currentDirectory = m_currentDirectory.parent_path();
				OnCurrentDirectoryhChanged();
			}
		}

		const float padding = 16.0f;
		const float thumbnailSize = 84.0f;
		const float thumbnailPadding = 4.0f;
		const float cellSize = thumbnailSize + padding;

		const float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0 , false);

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_currentDirectory))
		{
			const auto& path = directoryEntry.path();
			std::string filenameString = path.filename().string();

			ImGui::PushID(filenameString.c_str());
			Ref<Texture2D> icon = directoryEntry.is_directory() ? m_FolderIcon : m_FileIcon;

			std::string filepath = m_currentDirectory.string() + "\\" + filenameString;
			const bool bIsImageFile = filenameString.compare(filenameString.size() - 4, 4, ".png") == 0;

			if (bIsImageFile)
			{
				if (const Ref<Texture2D> foundIcon = GetImageInCache(filepath))
				{
					icon = foundIcon;
				}
				else
				{
					m_ImageIconCache.push_back(Texture2D::Create(filepath));
					icon = m_ImageIconCache.back();
				}
			}

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

			// Caclulate image size
			const float fAspectRatio = (float)icon->GetWidth() / (float)icon->GetHeight();
			const bool bWideImage = fAspectRatio >= 1.0f;
			const ImVec2 iconSize = bWideImage ? ImVec2(thumbnailSize, thumbnailSize * (1.0f / fAspectRatio)) : ImVec2(thumbnailSize * fAspectRatio, thumbnailSize);
			
			// Caclulate image padding needed to make square button
			const float fDimensionDifference = math::Abs(icon->GetWidth() - icon->GetHeight());
			const float fMaxDimension = bWideImage ? icon->GetWidth() : icon->GetHeight();
			const float fSmallDimensionPadding = (fDimensionDifference / fMaxDimension) * thumbnailSize * 0.5f;
			const ImVec2 iconPadding = bWideImage ? ImVec2(thumbnailPadding, fSmallDimensionPadding + thumbnailPadding) : ImVec2(fSmallDimensionPadding + thumbnailPadding, thumbnailPadding);
			
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, iconPadding);
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), iconSize, {0,1}, {1,0});
			ImGui::PopStyleVar();
			//ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0,1 }, { 1,0 });

			if (ImGui::BeginDragDropSource())
			{
				std::filesystem::path relativePath(path);
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
				{
					m_currentDirectory /= path.filename();
					OnCurrentDirectoryhChanged();
				}
				else
				{
				}
			}
			else if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			{
				if (!directoryEntry.is_directory())
				{
					const bool bIsTilesetFile = filenameString.compare(filenameString.size() - 3, 3, ".rt") == 0;
					if (bIsTilesetFile)
					{
						if (m_TileSetSelectedCallback)
						{
							std::string tilesetFilepath = m_currentDirectory.string() + "\\" + filenameString;
							m_TileSetSelectedCallback(tilesetFilepath);
						}
					}
				}
			}

			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();
			ImGui::PopID();
		}

		ImGui::Columns(1);

		ImGui::End();
	}

	void ContentBrowserPanel::OnCurrentDirectoryhChanged()
	{
		m_ImageIconCache.clear();
	}

	Ref<Texture2D> ContentBrowserPanel::GetImageInCache(const std::string& filepath)
	{
		for (Ref<Texture2D> icon : m_ImageIconCache)
		{
			if (icon->GetPath() == filepath)
			{
				return icon;
			}
		}

		return nullptr;
	}
}