#pragma once

#include "Rhombus/Math/Vector.h"
#include "Rhombus/Math/Math.h"

namespace rhombus
{
	class AABB
	{
	public:
		Vec3 c;
		Vec3 r;

		static bool TestAABBAABB(AABB a, AABB b)
		{
			int r;
			r = a.r.x + b.r.x; if ((uint32_t)(a.c.x - b.c.x + r) >= r + r) return false;
			r = a.r.y + b.r.y; if ((uint32_t)(a.c.y - b.c.y + r) >= r + r) return false;
			r = a.r.z + b.r.z; if ((uint32_t)(a.c.z - b.c.z + r) >= r + r) return false;
			return true;
		}
	};
}
