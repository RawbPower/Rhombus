#pragma once

#include <filesystem>

#include "Rhombus/Renderer/Texture.h"

namespace rhombus
{
	class ContentBrowserPanel
	{
	public:

		ContentBrowserPanel();

		void OnImGuiRender();
		void OnCurrentDirectoryhChanged();

		Ref<Texture2D> GetImageInCache(const std::string& filepath);

	private:
		std::filesystem::path m_baseDirectory;
		std::filesystem::path m_currentDirectory;

		Ref<Texture2D> m_FolderIcon;
		Ref<Texture2D> m_FileIcon;
		std::vector<Ref<Texture2D>> m_ImageIconCache;
	};
}