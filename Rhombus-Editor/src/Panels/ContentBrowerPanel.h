#pragma once

#include <filesystem>

#include "Rhombus/Renderer/Texture.h"

namespace rhombus
{
	typedef std::function<void(const std::string&)> TileSetSelectedCallback;

	class ContentBrowserPanel
	{
	public:

		ContentBrowserPanel();

		void OnImGuiRender();
		void OnCurrentDirectoryhChanged();

		Ref<Texture2D> GetImageInCache(const std::string& filepath);
		void SetTilesetSelectedCallback(TileSetSelectedCallback callback) { m_TileSetSelectedCallback = callback; }

	private:
		std::filesystem::path m_baseDirectory;
		std::filesystem::path m_currentDirectory;
		std::function<void(const std::string&)> m_TileSetSelectedCallback;

		Ref<Texture2D> m_FolderIcon;
		Ref<Texture2D> m_FileIcon;
		std::vector<Ref<Texture2D>> m_ImageIconCache;
	};
}