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

	Mat4 Translate(const Mat4& mat, const Vec3& translation);
	Mat4 Rotate(const Mat4& mat, float angle, const Vec3& v);
	Mat4 Scale(const Mat4& mat, const Vec3& scale);
	bool DecomposeTransform(const Mat4& transform, Vec3& outTranslation, Vec3& outRotation, Vec3& outScale);

	inline float Cos(float x) { return cos(x); }
	inline float Sin(float x) { return sin(x); }
	inline float Tan(float x) { return tan(x); }

	inline float Acos(float x) { return acos(x); }
	inline float Asin(float x) { return asin(x); }
	inline float Atan(float x) { return atan(x); }
	inline float Atan2(float y, float x) { return atan2(y, x); }

	inline float Pow(float x, int a) { return pow(x, a); }
	inline float Pow(float x, float a) { return pow(x, a); }
	inline float Sqrt(float x) { return sqrt(x); }

	inline float Round(float x) { return round(x); }
	inline float Ceil(float x) { return ceil(x); }
	inline float Floor(float x) { return floor(x); }
}