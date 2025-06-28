#pragma once

#include "Rhombus/ECS/System.h"
#include "Rhombus/Core/DeltaTime.h"

namespace rhombus
{
	class PlatformerPlayerControllerSystem : public System
	{
	public:
		PlatformerPlayerControllerSystem(Scene* scene) : System(scene)
		{
		}

		void Update(DeltaTime dt);
		void OnKeyPressed(int keycode, bool isRepeat);
		void OnGamepadButtonDown(int button);
		void ProcessJump();

	private:
	};

}
