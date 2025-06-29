#include "rbpch.h"
#include "AnimatorComponent.h"

#include "SpriteRendererComponent.h"

namespace rhombus
{
	void AnimatorComponent::Step(DeltaTime dt)
	{
		const AnimationClip& clip = GetCurrentAnimation();
		m_normalizedTime += dt / clip.m_duration;
		if (m_normalizedTime > 1.0f)
		{
			m_normalizedTime -= 1.0f;
		}

		float sampleNormTime = 0.0f;
		for (int i = 0; i < clip.m_samples.size(); i++)
		{
			sampleNormTime += clip.m_samples[i].m_duration / clip.m_duration;
			if (sampleNormTime > m_normalizedTime)
			{
				if (GetOwnerEntity().HasComponent<SpriteRendererComponent>())
				{
					SpriteRendererComponent& spriteRenderer = GetOwnerEntity().GetComponent<SpriteRendererComponent>();
					spriteRenderer.SetFrame(clip.m_samples[i].m_spriteFrame);
					break;
				}
			}
		}
	}

	void AnimatorComponent::RemoveAnimation(std::string name)
	{
		for (int i = 0; i < m_animations.size(); i++)
		{
			if (m_animations[i].m_name == name)
			{
				m_animations.erase(m_animations.begin() + i);
				break;
			}
		}
	}

	void AnimatorComponent::Play(std::string name)
	{
		for (int i = 0; i < m_animations.size(); i++)
		{
			AnimationClip clip = m_animations[i];
			if (clip.m_name == name)
			{
				m_currentAnimIndex = i;
				m_normalizedTime = 0.0f;
				break;
			}
		}
	}

	bool AnimatorComponent::IsPlaying(std::string name)
	{
		AnimationClip clip = m_animations[m_currentAnimIndex];
		if (clip.m_name == name)
		{
			return true;
		}

		return false;
	}
}