#pragma once

#include "Rhombus/ECS/System.h"
#include "Rhombus/Core/DeltaTime.h"

namespace rhombus
{
	class AnimationSystem : public System
	{
	public:
		AnimationSystem(Scene* scene) : System(scene)
		{
		}

		void Update(DeltaTime dt);

	private:
	};
}
