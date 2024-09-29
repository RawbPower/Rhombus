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

		uint32_t GameWidth = 480;
		uint32_t GameHeight = 270;
		uint32_t WindowWidth = 1920;
		uint32_t WindowHeight = 1080;
	};

	class Project
	{
	public:
		static const std::filesystem::path& GetProjectDirectory()
		{
			RB_CORE_ASSERT(s_ActiveProject, "No valid active project");
			return s_ActiveProject->m_ProjectDirectory;
		}

		static std::filesystem::path GetAssetDirectory()
		{
			RB_CORE_ASSERT(s_ActiveProject, "No valid active project");
			return GetProjectDirectory() / s_ActiveProject->m_Config.AssetDirectory;
		}

		static std::filesystem::path GetScriptDirectory()
		{
			RB_CORE_ASSERT(s_ActiveProject, "No valid active project");
			return GetProjectDirectory() / s_ActiveProject->m_Config.AssetDirectory / s_ActiveProject->m_Config.ScriptDirectory;
		}

		// TODO: move to asset manager when we have one
		static std::filesystem::path GetAssetFileSystemPath(const std::filesystem::path& path)
		{
			RB_CORE_ASSERT(s_ActiveProject, "No valid active project");
			return GetAssetDirectory() / path;
		}

		static std::string GetAssetFileLocalPath(const std::string& pathString)
		{
			RB_CORE_ASSERT(s_ActiveProject, "No valid active project");
			std::string assetPathString = GetAssetDirectory().string() + "\\";
			std::string localPathString = pathString;

			localPathString.erase(0, assetPathString.length());

			return localPathString;
		}

		static uint32_t GetGameWidth() { return s_ActiveProject->m_Config.GameWidth; }
		static uint32_t GetGameHeight() { return s_ActiveProject->m_Config.GameHeight; }

		static void SetGameResolution(uint32_t width, uint32_t height);

		ProjectConfig& GetConfig() { return m_Config; }

		static Ref<Project> GetActive() { return s_ActiveProject; }

		static Ref<Project> New();
		static Ref<Project> Load(const std::filesystem::path& path);
		static bool SaveActive(const std::filesystem::path& path);

	private:
		ProjectConfig m_Config;
		std::filesystem::path m_ProjectPath;
		std::filesystem::path m_ProjectDirectory;

		inline static Ref<Project> s_ActiveProject;
	};
}