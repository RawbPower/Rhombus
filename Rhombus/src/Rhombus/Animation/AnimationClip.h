#pragma once

#include "Rhombus/Core/Core.h"

#include <vector>

namespace rhombus
{
	class Texture2D;

	struct AnimationSample
	{
		float m_duration = 0.1f;
		Ref<Texture2D> m_sprite;
		int m_spriteFrame = 0;
	};

	struct AnimationClip
	{
		std::string m_name;
		bool m_looping = true;
		std::vector<AnimationSample> m_samples;
		float m_duration = 0.0f;
	};
}
