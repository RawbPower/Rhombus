#pragma once

#include "Rhombus/Core/Core.h"

#include <string>
#include <filesystem>

namespace rhombus
{
	struct ProjectConfig
	{
		std::string Name = "Untitled";
		std::filesystem::path StartScene;

		std::filesystem::path AssetDirectory;
		std::filesystem::path ScriptDirectory;
	};

	class Project
	{
	public:
		static const std::filesystem::path& GetProjectionDirectory()
		{
			RB_CORE_ASSERT(s_ActiveProject, "No valid active project");
			return s_ActiveProject->m_ProjectDirectory;
		}

		static std::filesystem::path GetAssetDirectory()
		{
			RB_CORE_ASSERT(s_ActiveProject, "No valid active project");
			return GetProjectionDirectory() / s_ActiveProject->m_Config.AssetDirectory;
		}

		// TODO: move to asset manager when we have one
		static std::filesystem::path GetAssetFileSystemPath(const std::filesystem::path& path)
		{
			RB_CORE_ASSERT(s_ActiveProject, "No valid active project");
			return GetAssetDirectory() / path;
		}

		ProjectConfig& GetConfig() { return m_Config; }

		static Ref<Project> GetActive() { return s_ActiveProject; }

		static Ref<Project> New();
		static Ref<Project> Load(const std::filesystem::path& path);
		static bool SaveActive(const std::filesystem::path& path);

	private:
		ProjectConfig m_Config;
		std::filesystem::path m_ProjectDirectory;

		inline static Ref<Project> s_ActiveProject;
	};
}