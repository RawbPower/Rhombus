#include "rbpch.h"
#include "AnimationSystem.h"

#include "Rhombus/ECS/Components/AnimatorComponent.h"
#include "Rhombus/ECS/Components/SpriteRendererComponent.h"

namespace rhombus
{
	void AnimationSystem::Update(DeltaTime dt)
	{
		for (Entity entity : GetEntities())
		{
			AnimatorComponent& animator = entity.GetComponent<AnimatorComponent>();

			const AnimationClip& clip = animator.GetCurrentAnimation();
			animator.m_normalizedTime += dt / clip.m_duration;
			if (animator.m_normalizedTime > 1.0f)
			{
				animator.m_normalizedTime -= 1.0f;
			}

			float sampleNormTime = 0.0f;
			for (int i = 0; i < clip.m_samples.size(); i++)
			{
				sampleNormTime += clip.m_samples[i].m_duration / clip.m_duration;
				if (sampleNormTime > animator.m_normalizedTime)
				{
					if (entity.HasComponent<SpriteRendererComponent>())
					{
						SpriteRendererComponent& spriteRenderer = entity.GetComponent<SpriteRendererComponent>();
						spriteRenderer.SetFrame(clip.m_samples[i].m_spriteFrame);
						break;
					}
				}
			}
		}
	}
}