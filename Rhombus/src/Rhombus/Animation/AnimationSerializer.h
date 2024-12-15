#pragma once

#include "Rhombus/Scenes/Entity.h"

#include <filesystem>

namespace rhombus
{
	class AnimationSerializer
	{
	public:
		static void SerializeAnimations(const std::string& filepath, Entity entity);
		static bool DeserializeAnimations(const std::string& filepath, Entity entity);

	private:
		AnimationSerializer();
	};
}

