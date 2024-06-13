#pragma once

#include "Matrix.h"
#include "Vector.h"
#include "Quat.h"

namespace rhombus::math
{
	// TODO: Check if this is the best way to make these consts
	static const float PI = 3.14159f;
	static const float RadToDeg = 57.29578f;
	static const float DegToRad = 0.017453f;

	float Lerp(float x, float y, float a);
	Vec2 Lerp(const Vec2& x, const Vec2& y, const Vec2& a);
	Vec3 Lerp(const Vec3& x, const Vec3& y, const Vec3& a);
	Vec4 Lerp(const Vec4& x, const Vec4& y, const Vec4& a);

	Vec3 Rotate(const Quat& rotation, const Vec3& axis);

	Mat4 Translate(const Mat4& mat, const Vec3& translation);
	Mat4 Rotate(const Mat4& mat, float angle, const Vec3& v);
	Mat4 Scale(const Mat4& mat, const Vec3& scale);
	bool DecomposeTransform(const Mat4& transform, Vec3& outTranslation, Vec3& outRotation, Vec3& outScale);
}