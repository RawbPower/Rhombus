#include "rbpch.h"
#include "TweeningSystem.h"

#include "Rhombus/ECS/Components/TweenComponent.h"

namespace rhombus
{
	void TweeningSystem::UpdateTweens(DeltaTime dt)
	{
		for (Entity entity : GetEntities())
		{
			auto& tweenComponent = entity.GetComponent<TweenComponent>();

			Ref<Tween> tween = tweenComponent.GetTween();
			if (tween)
			{
				tween->Step(dt);
				if (tween->GetIsFinished())
				{
					entity.RemoveComponent<TweenComponent>();
					continue;
				}
			}
		}
	}
}