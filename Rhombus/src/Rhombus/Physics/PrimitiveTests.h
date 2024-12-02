#pragma once

#include "Rhombus/Math/Vector.h"
#include "AABB.h"
#include "Sphere.h"

namespace rhombus
{
	class PrimitiveTests
	{
	public:
		// Retrun true if sphere s intersects AABB b, false otherwise
		static bool TestSphereAABB(Sphere s, AABB b)
		{
			// Compute square distance between sphere center and AABB
			float sqDist = AABB::SqDistPointAABB(s.c, b);

			// Sphere and AABB intersect if the (squared) distance
			// between them is less than the (squared) sphere radius
			return sqDist < s.r * s.r;
		}

		// Retrun true if sphere s intersects AABB b, false otherwise
		// The point p on the AABB closest to the sphere center is also returned
		static bool TestSphereAABB(Sphere s, AABB b, Vec3& p)
		{
			// Find point p on AABB closest the the sphere center
			AABB::ClosestPtPointAABB(s.c, b, p);

			// Sphere and AABB intersect if the (squared) distance
			// between them is less than the (squared) sphere radius
			Vec3 v = p - s.c;
			return Vec3::Dot(v, v) < s.r * s.r;
		}
	};
}
