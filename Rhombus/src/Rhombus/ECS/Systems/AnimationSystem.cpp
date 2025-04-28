#include "rbpch.h"
#include "AnimationSystem.h"

#include "Rhombus/ECS/Components/AnimatorComponent.h"
#include "Rhombus/ECS/Components/SpriteRendererComponent.h"

namespace rhombus
{
	void AnimationSystem::Init()
	{
		for (Entity entity : GetEntities())
		{
			AnimatorComponent& animator = entity.GetComponent<AnimatorComponent>();
			animator.Reset();
		}
	}

	void AnimationSystem::Update(DeltaTime dt)
	{
		for (Entity entity : GetEntities())
		{
			AnimatorComponent& animator = entity.GetComponent<AnimatorComponent>();
			animator.Step(dt);
		}
	}
}