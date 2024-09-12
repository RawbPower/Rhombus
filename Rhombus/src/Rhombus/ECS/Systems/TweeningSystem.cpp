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

			bool areAllTweensFinished = true;
			for (int i = 0; i < tweenComponent.GetTweenCount(); i++)
			{
				Ref<Tween> tween = tweenComponent.GetTween(i);
				if (tween)
				{
					tween->Step(dt);
					if (!tween->GetIsFinished())
					{
						areAllTweensFinished = false;
					}
				}
			}

			if (areAllTweensFinished)
			{
				entity.RemoveComponent<TweenComponent>();
			}
		}
	}
}