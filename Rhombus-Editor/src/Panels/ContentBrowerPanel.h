#pragma once

#include <filesystem>

namespace rhombus
{
	class ContentBrowserPanel
	{
	public:

		ContentBrowserPanel();

		void OnImGuiRender();

	private:
		std::filesystem::path m_currentDirectory;
	};
}