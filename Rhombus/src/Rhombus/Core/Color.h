#pragma once

#include "Rhombus/Math/Vector.h"

namespace rhombus
{
	class Color
	{
	public:
		Color();
		Color(const float value);
		Color(const Color& rhs);
		Color(const Vec4& rhs);
		Color(float R, float G, float B, float A);
		Color(const float* rhs);
		Color& operator = (const Color& rhs);

		bool operator == (const Color& rhs) const;
		bool operator != (const Color& rhs) const;
		Color operator + (const Color& rhs) const;
		const Color& operator += (const Color& rhs);
		const Color& operator -= (const Color& rhs);
		const Color& operator *= (const Color& rhs);
		const Color& operator /= (const Color& rhs);
		Color operator - (const Color& rhs) const;
		Color operator * (const float rhs) const;
		Color operator / (const float rhs) const;
		float operator [] (const int idx) const;
		float& operator [] (const int idx);

		operator Vec4() const;

		Vec3 GetRGB() { return Vec3(r, g, b); }

		const float* ToPtr() const { return &r; }
		float* ToPtr() { return &r; }

	public:
		float r;
		float g;
		float b;
		float a;
	};

	inline Color::Color() :
		r(0.0f),
		g(0.0f),
		b(0.0f),
		a(0.0f)
	{
	}

	inline Color::Color(const float value) :
		r(value),
		g(value),
		b(value),
		a(value)
	{
	}

	inline Color::Color(const Color& rhs) :
		r(rhs.r),
		g(rhs.g),
		b(rhs.b),
		a(rhs.a)
	{
	}

	inline Color::Color(const Vec4& rhs) :
		r(rhs.x),
		g(rhs.y),
		b(rhs.z),
		a(rhs.w)
	{
	}

	inline Color::Color(float R, float G, float B, float A) :
		r(R),
		g(G),
		b(B),
		a(A)
	{
	}

	inline Color::Color(const float* rhs)
	{
		r = rhs[0];
		g = rhs[1];
		b = rhs[2];
		a = rhs[3];
	}

	inline Color& Color::operator = (const Color& rhs)
	{
		r = rhs.r;
		g = rhs.g;
		b = rhs.b;
		a = rhs.a;
		return *this;
	}

	inline bool Color::operator == (const Color& rhs) const
	{
		if (r != rhs.r)
		{
			return false;
		}

		if (g != rhs.g)
		{
			return false;
		}

		if (b != rhs.b)
		{
			return false;
		}

		if (a != rhs.a)
		{
			return false;
		}

		return true;
	}

	inline bool Color::operator != (const Color& rhs) const
	{
		if (*this == rhs)
		{
			return false;
		}

		return true;
	}

	inline Color Color::operator + (const Color& rhs) const
	{
		Color temp;
		temp.r = r + rhs.r;
		temp.g = g + rhs.g;
		temp.b = b + rhs.b;
		temp.a = a + rhs.a;
		return temp;
	}

	inline const Color& Color::operator += (const Color& rhs)
	{
		r += rhs.r;
		g += rhs.g;
		b += rhs.b;
		a += rhs.a;
		return *this;
	}

	inline const Color& Color::operator -= (const Color& rhs)
	{
		r -= rhs.r;
		g -= rhs.g;
		b -= rhs.b;
		a -= rhs.a;
		return *this;
	}

	inline const Color& Color::operator *= (const Color& rhs)
	{
		r *= rhs.r;
		g *= rhs.g;
		b *= rhs.b;
		a *= rhs.a;
		return *this;
	}

	inline const Color& Color::operator /= (const Color& rhs)
	{
		r /= rhs.r;
		g /= rhs.g;
		b /= rhs.b;
		a /= rhs.a;
		return *this;
	}

	inline Color Color::operator - (const Color& rhs) const
	{
		Color temp;
		temp.r = r - rhs.r;
		temp.g = g - rhs.g;
		temp.b = b - rhs.b;
		temp.a = a - rhs.a;
		return temp;
	}

	inline Color Color::operator * (const float rhs) const
	{
		Color temp;
		temp.r = r * rhs;
		temp.g = g * rhs;
		temp.b = b * rhs;
		temp.a = a * rhs;
		return temp;
	}

	inline Color Color::operator / (const float rhs) const
	{
		Color temp;
		temp.r = r / rhs;
		temp.g = g / rhs;
		temp.b = b / rhs;
		temp.a = a / rhs;
		return temp;
	}

	inline float Color::operator [] (const int idx) const
	{
		RB_CORE_ASSERT(idx >= 0 && idx < 4, "Index {0} is out of bounds of Color", idx);
		return (&r)[idx];
	}

	inline float& Color::operator [] (const int idx)
	{
		RB_CORE_ASSERT(idx >= 0 && idx < 4, "Index {0} is out of bounds of Color", idx);
		return (&r)[idx];
	}

	inline Color::operator Vec4() const
	{
		Vec4 v;
		v.x = r;
		v.y = g;
		v.z = b;
		v.z = a;
		return v;
	}
}
