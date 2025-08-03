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
		void OnKeyReleased(int keycode);
		void OnGamepadButtonDown(int button);
		void OnGamepadButtonUp(int button);
		void ProcessJump();
		void CancelJump();

	private:
	};

}
