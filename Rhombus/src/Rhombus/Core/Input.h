#pragma once

#include "Rhombus/Core/Core.h"

namespace rhombus {

	class RB_API Input
	{
	public:
		static bool IsKeyPressed(int keycode);
		static bool IsMouseButtonPressed(int button);
		static Vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}