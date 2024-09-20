#pragma once

namespace rhombus::easing
{
	static float linear(float t, float b, float c, float d) {
		return c * t / d + b;
	}
}

