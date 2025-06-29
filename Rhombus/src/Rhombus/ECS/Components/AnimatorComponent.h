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

		void Reset() { m_normalizedTime = 0.0f; m_currentAnimIndex = 0; }

		void Step(DeltaTime dt);

		void AddAnimation(AnimationClip clip) { m_animations.push_back(clip); }
		void RemoveAnimation(std::string name);
		const AnimationClip& GetCurrentAnimation() const { return m_animations[m_currentAnimIndex]; }

		void Play(std::string name);
		bool IsPlaying(std::string name);

		int GetAnimationCount() const { return m_animations.size(); }
		AnimationClip GetAnimationClip(int index) const { return m_animations[index]; }
		AnimationClip& GetAnimationClipRef(int index) { return m_animations[index]; }

		std::string m_filePath;
		float m_normalizedTime = 0.0f;

	private:
		std::vector<AnimationClip> m_animations;
		uint32_t m_currentAnimIndex = 0;
	};
}
