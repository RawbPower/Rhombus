#include "rbpch.h"

#include "ContentBrowerPanel.h"

#include "Rhombus/Core/Application.h"
#include "Rhombus/Project/Project.h"

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
			}
		}

		float padding = 16.0f;
		float thumbnailSize = 64.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
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
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0,1 }, { 1,0 });

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
				}
				else
				{

				}
			}

			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();
			ImGui::PopID();
		}

		ImGui::Columns(1);

		ImGui::End();
	}
}