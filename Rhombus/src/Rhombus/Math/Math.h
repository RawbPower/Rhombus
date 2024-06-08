#pragma once

#include "Vector.h"
#include "Matrix.h"

namespace rhombus::math
{
	bool DecomposeTransform(const Mat4& transform, Vec3& outTranslation, Vec3& outRotation, Vec3& outScale);
}