#pragma once

#include "Rhombus/Math/Vector.h"

namespace rhombus
{
	class Sphere
	{
	public:
		Sphere() = default;

		Vec3 c;		// Sphere center
		float r;	// Sphere radius
	};
}
