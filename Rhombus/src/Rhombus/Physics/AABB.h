#pragma once

#include "Rhombus/Math/Vector.h"
#include "Rhombus/Math/Math.h"
#include "Sphere.h"

namespace rhombus
{
	class AABB
	{
	public:
		Vec3 c;
		Vec3 r;

		// Given point p return the point q on or in the AABB b that is closes to b
		static void ClosestPtPointAABB(Vec3 p, AABB b, Vec3& q)
		{
			Vec3 bmin = b.c - b.r;
			Vec3 bmax = b.c + b.r;

			// For each coordinate axis, if the point coordinate value is
			// outside box, clamp it to the box, else keep it as is
			for (int i = 0; i < 3; i++)
			{
				float v = p[i];
				if (v < bmin[i]) v = bmin[i];		// v = max(v, bmin[i])
				if (v > bmax[i]) v = bmax[i];		// v = min(v, bmax[i])
				q[i] = v;
			}
		}

		// Computes the square distance between a point p and an AABB b
		static float SqDistPointAABB(Vec3 p, AABB b)
		{
			Vec3 bmin = b.c - b.r;
			Vec3 bmax = b.c + b.r;

			float sqDist = 0.0f;

			for (int i = 0; i < 3; i++)
			{
				// For each axis cound any excess distance outside box extents
				float v = p[i];
				if (v < bmin[i]) sqDist += (bmin[i] - v) * (bmin[i] - v);
				if (v > bmax[i]) sqDist += (v - bmax[i]) * (v - bmax[i]);
			}

			return sqDist;
		}

		// Not working for collisions where b is above a
		/*static bool TestAABBAABB(AABB a, AABB b)
		{
			int r;
			r = a.r.x + b.r.x; if ((uint32_t)(a.c.x - b.c.x + r) >= r + r) return false;
			r = a.r.y + b.r.y; if ((uint32_t)(a.c.y - b.c.y + r) >= r + r) return false;
			r = a.r.z + b.r.z; if ((uint32_t)(a.c.z - b.c.z + r) >= r + r) return false;
			return true;
		}*/

		static bool TestAABBAABB(AABB a, AABB b)
		{
			if (math::Abs(a.c.x - b.c.x) >= (a.r.x + b.r.x)) return false;
			if (math::Abs(a.c.y - b.c.y) >= (a.r.y + b.r.y)) return false;
			if (math::Abs(a.c.z - b.c.z) >= (a.r.z + b.r.z)) return false;
			return true;
		}

		// Retrun true if sphere s intersects AABB b, false otherwise
		static bool TestSphereAABB(Sphere s, AABB b)
		{
			// Compute square distance between sphere center and AABB
			float sqDist = SqDistPointAABB(s.c, b);

			// Sphere and AABB intersect if the (squared) distance
			// between them is less than the (squared) sphere radius
			return sqDist < s.r * s.r;
		}

		// Retrun true if sphere s intersects AABB b, false otherwise
		// The point p on the AABB closest to the sphere center is also returned
		static bool TestSphereAABB(Sphere s, AABB b, Vec3& p)
		{
			// Find point p on AABB closest the the sphere center
			ClosestPtPointAABB(s.c, b, p);

			// Sphere and AABB intersect if the (squared) distance
			// between them is less than the (squared) sphere radius
			Vec3 v = p - s.c;
			return Vec3::Dot(v, v) < s.r * s.r;
		}
	};
}
