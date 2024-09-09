#pragma once

#include "Rhombus/ECS/System.h"
#include "Rhombus/Core/DeltaTime.h"

namespace rhombus
{
	class TweeningSystem : public System
	{
	public:
		TweeningSystem(Scene* scene) : System(scene)
		{
		}

		void UpdateTweens(DeltaTime dt);
	};

}

