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

	private:
		std::filesystem::path m_baseDirectory;
		std::filesystem::path m_currentDirectory;

		Ref<Texture2D> m_FolderIcon;
		Ref<Texture2D> m_FileIcon;
	};
}