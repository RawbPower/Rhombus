#include "rbpch.h"
#include "Project.h"

#include "ProjectSerializer.h"

namespace rhombus
{
	Ref<Project> Project::New()
	{
		s_ActiveProject = CreateRef<Project>();
		return s_ActiveProject;
	}

	Ref<Project> Project::Load(const std::filesystem::path& path)
	{
		Ref<Project> project = CreateRef<Project>();

		ProjectSerializer serializer(project);
		if (serializer.Deserialize(path))
		{
			project->m_ProjectPath = path;
			project->m_ProjectDirectory = path.parent_path();
			s_ActiveProject = project;
			return s_ActiveProject;
		}

		return nullptr;
	}

	bool Project::SaveActive(const std::filesystem::path& path)
	{
		ProjectSerializer serializer(s_ActiveProject);
		if (serializer.Serialize(path))
		{
			s_ActiveProject->m_ProjectDirectory = path.parent_path();
			return true;
		}

		return false;
	}

	void Project::SetGameResolution(uint32_t width, uint32_t height)
	{ 
		s_ActiveProject->m_Config.GameWidth = width; 
		s_ActiveProject->m_Config.GameHeight = height; 

		SaveActive(s_ActiveProject->m_ProjectPath);
	}
}