#pragma once

#include "Component.h"
#include "Rhombus/Animation/AnimationClip.h"

namespace rhombus
{
	class AnimatorComponent : public ComponentBase
	{
	public:
		AnimatorComponent() = default;
		AnimatorComponent(const AnimatorComponent& other) = default;

		void AddAnimation(AnimationClip clip) { m_animations.push_back(clip); }
		const AnimationClip& GetCurrentAnimation() const { return m_animations[m_currentAnimIndex]; }

		std::string m_filePath;
		float m_normalizedTime = 0.0f;

	private:
		std::vector<AnimationClip> m_animations;
		uint32_t m_currentAnimIndex = 0;
	};
}
