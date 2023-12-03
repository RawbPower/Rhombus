#pragma once

#include "Scene.h"

namespace rhombus
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		void Serialize(const std::string& filepath);				// Serialize YAML text
		void SerializeRuntime(const std::string& filepath);			// Serialize the binaries

		bool Deserialize(const std::string& filepath);
		bool DeserializeRuntime(const std::string& filepath);

	private:
		Ref<Scene> m_scene;
	};
}