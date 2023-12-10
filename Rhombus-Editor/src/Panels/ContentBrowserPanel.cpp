#include "rbpch.h"

#include "ContentBrowerPanel.h"

#include <imgui/imgui.h>

namespace rhombus
{
	// Once we have projects, change this
	static std::filesystem::path s_AssetPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_currentDirectory(s_AssetPath)
	{
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (m_currentDirectory != std::filesystem::path(s_AssetPath))
		{
			if (ImGui::Button("../"))
			{
				m_currentDirectory = m_currentDirectory.parent_path();
			}
		}

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_currentDirectory))
		{
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, s_AssetPath);
			std::string filenameString = relativePath.filename().string();
			if (directoryEntry.is_directory())
			{
				if (ImGui::Button(filenameString.c_str()))
				{
					m_currentDirectory /= path.filename();
				}
			}
			else
			{
				if (ImGui::Button(filenameString.c_str()))
				{
				}
			}
		}

		ImGui::End();
	}
}