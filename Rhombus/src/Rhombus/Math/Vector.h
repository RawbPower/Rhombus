#pragma once

#include "Rhombus/Core/Log.h"

#include <glm/glm.hpp>

namespace rhombus
{
	// Forward declarations
	class Vec2;
	class Vec3;
	class Vec4;

	/* 
	===============================
	Vec2
	===============================
	*/
	class Vec2
	{
	public:
		Vec2();
		Vec2(const float value);
		Vec2(const Vec2& rhs);
		Vec2(float X, float Y);
		Vec2(const float* xy);
		Vec2& operator = ( const Vec2& rhs );

		bool operator == (const Vec2& rhs) const;
		bool operator != (const Vec2& rhs) const;
		Vec2 operator + (const Vec2& rhs) const;
		const Vec2& operator += (const Vec2& rhs);
		const Vec2& operator -= (const Vec2& rhs);
		Vec2 operator - (const Vec2& rhs) const;
		Vec2 operator * (const float rhs) const;
		Vec2 operator / (const float rhs) const;
		const Vec2& operator *= (const float rhs);
		const Vec2& operator /= (const float rhs);
		float operator [] (const int idx) const;
		float& operator [] (const int idx);

		void Zero() { x = 0.0f; y = 0.0f; }

		const Vec2& Normalize();
		float GetMagnitude() const;
		float GetMag2() const { return Dot(*this); }
		bool IsValid() const;
		float Dot(const Vec2& rhs) const { return x * rhs.x + y * rhs.y; }

		static float Dot(const Vec2& a, const Vec2& b) { return a.Dot(b); }

		const float* ToPtr() const { return &x; }

	public:
		float x;
		float y;
	};


	/*
	===============================
	Vec3
	===============================
	*/
	class Vec3
	{
	public:
		Vec3();
		Vec3(const float value);
		Vec3(const Vec3& rhs);
		Vec3(const Vec4& rhs);
		Vec3(float X, float Y, float Z);
		Vec3(const float* xyz);
		Vec3& operator = (const Vec3& rhs);
		Vec3& operator = (const float* rhs);

		bool operator == (const Vec3& rhs) const;
		bool operator != (const Vec3& rhs) const;
		Vec3 operator + (const Vec3& rhs) const;
		const Vec3& operator += (const Vec3& rhs);
		const Vec3& operator -= (const Vec3& rhs);
		Vec3 operator - (const Vec3& rhs) const;
		Vec3 operator * (const float rhs) const;
		Vec3 operator / (const float rhs) const;
		const Vec3& operator *= (const float rhs);
		const Vec3& operator /= (const float rhs);
		float operator [] (const int idx) const;
		float& operator [] (const int idx);

		operator glm::vec3() const;

		void Zero() { x = 0.0f; y = 0.0f; z = 0.0f; }

		const Vec3& Normalize();
		float GetMagnitude() const;
		float GetMag2() const { return Dot(*this); }
		bool IsValid() const;
		void GetOrtho(Vec3& u, Vec3& v) const;

		Vec3 Cross(const Vec3& rhs) const;
		float Dot(const Vec3& rhs) const;

		static Vec3 Cross(const Vec3& a, const Vec3& b);
		static float Dot(const Vec3& a, const Vec3& b);

		const float* ToPtr() const { return &x; }

	public:
		float x;
		float y;
		float z;
	};


	/*
	===============================
	Vec4
	===============================
	*/
	class Vec4
	{
	public:
		Vec4();
		Vec4(const float value);
		Vec4(const Vec4& rhs);
		Vec4(float X, float Y, float Z, float W);
		Vec4(const float* rhs);
		Vec4& operator = (const Vec4& rhs);

		bool operator == (const Vec4& rhs) const;
		bool operator != (const Vec4& rhs) const;
		Vec4 operator + (const Vec4& rhs) const;
		const Vec4& operator += (const Vec4& rhs);
		const Vec4& operator -= (const Vec4& rhs);
		const Vec4& operator *= (const Vec4& rhs);
		const Vec4& operator /= (const Vec4& rhs);
		Vec4 operator - (const Vec4& rhs) const;
		Vec4 operator * (const float rhs) const;
		Vec4 operator / (const float rhs) const;
		float operator [] (const int idx) const;
		float& operator [] (const int idx);

		operator glm::vec4() const;

		Vec3 GetXYZ() { return Vec3(x, y, z); }

		void Zero() { x = 0.0f; y = 0.0f; z = 0.0f; w = 0.0f; }

		const Vec4& Normalize();
		float GetMagnitude() const;
		float GetMag2() const { return Dot(*this); }
		bool IsValid() const;

		float Dot(const Vec4& rhs) const { return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w; }
		static float Dot(const Vec4& a, const Vec4& b) { return a.Dot(b); }

		const float* ToPtr() const { return &x; }
		float* ToPtr() { return &x; }

	public:
		float x;
		float y;
		float z;
		float w;
	};


	/*
	===============================
	Vec2
	===============================
	*/
	inline Vec2::Vec2() :
		x(0.0f),
		y(0.0f)
	{
	}

	inline Vec2::Vec2(const float value) :
		x(value),
		y(value)
	{
	}

	inline Vec2::Vec2(const Vec2& rhs) :
		x(rhs.x),
		y(rhs.y)
	{
	}

	inline Vec2::Vec2(float X, float Y) :
		x(X),
		y(Y)
	{
	}

	inline Vec2::Vec2(const float* xy) :
		x(xy[0]),
		y(xy[1])
	{
	}

	inline Vec2& Vec2::operator = (const Vec2& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		return *this;
	}

	inline bool Vec2::operator == (const Vec2& rhs) const
	{
		if (x != rhs.x)
		{
			return false;
		}

		if (y != rhs.y)
		{
			return false;
		}

		return true;
	}

	inline bool Vec2::operator != (const Vec2& rhs) const
	{
		if (*this == rhs)
		{
			return false;
		}

		return true;
	}

	inline Vec2 Vec2::operator + (const Vec2& rhs) const
	{
		Vec2 temp;
		temp.x = x + rhs.x;
		temp.y = y + rhs.y;
		return temp;
	}

	inline const Vec2& Vec2::operator += (const Vec2& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	inline const Vec2& Vec2::operator -= (const Vec2& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	inline Vec2 Vec2::operator - (const Vec2& rhs) const
	{
		Vec2 temp;
		temp.x = x - rhs.x;
		temp.y = y - rhs.y;
		return temp;
	}

	inline Vec2 Vec2::operator * (const float rhs) const
	{
		Vec2 temp;
		temp.x = x * rhs;
		temp.y = y * rhs;
		return temp;
	}

	inline Vec2 Vec2::operator / (const float rhs) const
	{
		Vec2 temp;
		temp.x = x / rhs;
		temp.y = y / rhs;
		return temp;
	}

	inline const Vec2& Vec2::operator *= (const float rhs)
	{
		x *= rhs;
		y *= rhs;
		return *this;
	}

	inline const Vec2& Vec2::operator /= (const float rhs)
	{
		x /= rhs;
		y /= rhs;
		return *this;
	}

	inline float Vec2::operator [] (const int idx) const
	{
		RB_CORE_ASSERT(idx >= 0 && idx < 2, "Index {0} is out of bounds of Vec2", idx);
		return (&x)[idx];
	}

	inline float& Vec2::operator [] (const int idx)
	{
		RB_CORE_ASSERT(idx >= 0 && idx < 2, "Index {0} is out of bounds of Vec2", idx);
		return (&x)[idx];
	}

	inline const Vec2& Vec2::Normalize()
	{
		float mag = GetMagnitude();
		float invMag = 1.0f / mag;
		if (0.0f * invMag == 0.0f * invMag)
		{
			x = x * invMag;
			y = y * invMag;
		}

		return *this;
	}

	inline float Vec2::GetMagnitude() const
	{
		float mag;

		mag = x * x + y * y;
		mag = sqrtf(mag);
		return mag;
	}

	inline bool Vec2::IsValid() const
	{
		if (x * 0.0f != x * 0.0f)
		{
			// x is NaN or Inf
			return false;
		}

		if (y * 0.0f != y * 0.0f)
		{
			// y is NaN or Inf
			return false;
		}

		return true;
	}


	/*
	===============================
	Vec3
	===============================
	*/
	inline Vec3::Vec3() :
		x(0.0f),
		y(0.0f),
		z(0.0f)
	{
	}

	inline Vec3::Vec3(const float value) :
		x(value),
		y(value),
		z(value)
	{
	}

	inline Vec3::Vec3(const Vec3& rhs) :
		x(rhs.x),
		y(rhs.y),
		z(rhs.z)
	{
	}

	inline Vec3::Vec3(const Vec4& rhs) :
		x(rhs.x),
		y(rhs.y),
		z(rhs.z)
	{
	}

	inline Vec3::Vec3(float X, float Y, float Z) :
		x(X),
		y(Y),
		z(Z)
	{
	}

	inline Vec3::Vec3(const float* xyz) :
		x(xyz[0]),
		y(xyz[1]),
		z(xyz[2])
	{
	}

	inline Vec3& Vec3::operator = (const Vec3& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}

	inline Vec3& Vec3::operator = (const float* rhs)
	{
		x = rhs[0];
		y = rhs[1];
		z = rhs[2];
		return *this;
	}

	inline bool Vec3::operator == (const Vec3& rhs) const
	{
		if (x != rhs.x)
		{
			return false;
		}

		if (y != rhs.y)
		{
			return false;
		}

		if (z != rhs.z)
		{
			return false;
		}

		return true;
	}

	inline bool Vec3::operator != (const Vec3& rhs) const
	{
		if (*this == rhs)
		{
			return false;
		}

		return true;
	}

	inline Vec3 Vec3::operator + (const Vec3& rhs) const
	{
		Vec3 temp;
		temp.x = x + rhs.x;
		temp.y = y + rhs.y;
		temp.z = z + rhs.z;
		return temp;
	}

	inline const Vec3& Vec3::operator += (const Vec3& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	inline const Vec3& Vec3::operator -= (const Vec3& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	inline Vec3 Vec3::operator - (const Vec3& rhs) const
	{
		Vec3 temp;
		temp.x = x - rhs.x;
		temp.y = y - rhs.y;
		temp.z = z - rhs.z;
		return temp;
	}

	inline Vec3 Vec3::operator * (const float rhs) const
	{
		Vec3 temp;
		temp.x = x * rhs;
		temp.y = y * rhs;
		temp.z = z * rhs;
		return temp;
	}

	inline Vec3 Vec3::operator / (const float rhs) const
	{
		Vec3 temp;
		temp.x = x / rhs;
		temp.y = y / rhs;
		temp.z = z / rhs;
		return temp;
	}

	inline const Vec3& Vec3::operator *= (const float rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}

	inline const Vec3& Vec3::operator /= (const float rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}

	inline float Vec3::operator [] (const int idx) const
	{
		RB_CORE_ASSERT(idx >= 0 && idx < 3, "Index {0} is out of bounds of Vec3", idx);
		return (&x)[idx];
	}

	inline float& Vec3::operator [] (const int idx)
	{
		RB_CORE_ASSERT(idx >= 0 && idx < 3, "Index {0} is out of bounds of Vec3", idx);
		return (&x)[idx];
	}

	inline Vec3::operator glm::vec3() const
	{
		glm::vec3 v;
		v.x = x;
		v.y = y;
		v.z = z;
		return v;
	}

	inline const Vec3& Vec3::Normalize()
	{
		float mag = GetMagnitude();
		float invMag = 1.0f / mag;
		if (0.0f * invMag == 0.0f * invMag)
		{
			x = x * invMag;
			y = y * invMag;
			z = z * invMag;
		}

		return *this;
	}

	inline float Vec3::GetMagnitude() const
	{
		float mag;

		mag = x * x + y * y + z * z;
		mag = sqrtf(mag);
		return mag;
	}

	inline bool Vec3::IsValid() const
	{
		if (x * 0.0f != x * 0.0f)
		{
			// x is NaN or Inf
			return false;
		}

		if (y * 0.0f != y * 0.0f)
		{
			// y is NaN or Inf
			return false;
		}

		if (z * 0.0f != z * 0.0f)
		{
			// z is NaN or Inf
			return false;
		}

		return true;
	}

	inline void Vec3::GetOrtho(Vec3& u, Vec3& v) const
	{
		Vec3 n = *this;
		n.Normalize();

		const Vec3 w = (n.z * n.z > 0.9f * 0.9f) ? Vec3(1, 0, 0) : Vec3(0, 0, 1);
		u = w.Cross(n);
		u.Normalize();

		v = n.Cross(u);
		v.Normalize();
		u = v.Cross(n);
		u.Normalize();
	}

	inline Vec3 Vec3::Cross(const Vec3& rhs) const
	{
		// This cross product is A x B, where this is A and rhs is B
		Vec3 temp;
		temp.x = (y * rhs.z) - (rhs.y * z);
		temp.y = (rhs.x * z) - (x * rhs.z);
		temp.z = (x * rhs.y) - (rhs.x * y);
		return temp;
	}

	inline float Vec3::Dot(const Vec3& rhs) const
	{
		float temp = (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
		return temp;
	}

	inline Vec3 Vec3::Cross(const Vec3& a, const Vec3& b)
	{
		return a.Cross(b);
	}

	inline float Vec3::Dot(const Vec3& a, const Vec3& b)
	{
		return a.Dot(b);
	}


	/*
	===============================
	Vec4
	===============================
	*/
	inline Vec4::Vec4() :
		x(0.0f),
		y(0.0f),
		z(0.0f),
		w(0.0f)
	{
	}

	inline Vec4::Vec4(const float value) :
		x(value),
		y(value),
		z(value),
		w(value)
	{
	}

	inline Vec4::Vec4(const Vec4& rhs) :
		x(rhs.x),
		y(rhs.y),
		z(rhs.z),
		w(rhs.w)
	{
	}

	inline Vec4::Vec4(float X, float Y, float Z, float W) :
		x(X),
		y(Y),
		z(Z),
		w(W)
	{
	}

	inline Vec4::Vec4(const float* rhs)
	{
		x = rhs[0];
		y = rhs[1];
		z = rhs[2];
		w = rhs[3];
	}

	inline Vec4& Vec4::operator = (const Vec4& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;
		return *this;
	}

	inline bool Vec4::operator == (const Vec4& rhs) const
	{
		if (x != rhs.x)
		{
			return false;
		}

		if (y != rhs.y)
		{
			return false;
		}

		if (z != rhs.z)
		{
			return false;
		}

		if (w != rhs.w)
		{
			return false;
		}

		return true;
	}

	inline bool Vec4::operator != (const Vec4& rhs) const
	{
		if (*this == rhs)
		{
			return false;
		}

		return true;
	}

	inline Vec4 Vec4::operator + (const Vec4& rhs) const
	{
		Vec4 temp;
		temp.x = x + rhs.x;
		temp.y = y + rhs.y;
		temp.z = z + rhs.z;
		temp.w = w + rhs.w;
		return temp;
	}

	inline const Vec4& Vec4::operator += (const Vec4& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
		return *this;
	}

	inline const Vec4& Vec4::operator -= (const Vec4& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
		return *this;
	}

	inline const Vec4& Vec4::operator *= (const Vec4& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		w *= rhs.w;
		return *this;
	}

	inline const Vec4& Vec4::operator /= (const Vec4& rhs)
	{
		x /= rhs.x;
		y /= rhs.y;
		z /= rhs.z;
		w /= rhs.w;
		return *this;
	}

	inline Vec4 Vec4::operator - (const Vec4& rhs) const
	{
		Vec4 temp;
		temp.x = x - rhs.x;
		temp.y = y - rhs.y;
		temp.z = z - rhs.z;
		temp.w = w - rhs.w;
		return temp;
	}

	inline Vec4 Vec4::operator * (const float rhs) const
	{
		Vec4 temp;
		temp.x = x * rhs;
		temp.y = y * rhs;
		temp.z = z * rhs;
		temp.w = w * rhs;
		return temp;
	}

	inline Vec4 Vec4::operator / (const float rhs) const
	{
		Vec4 temp;
		temp.x = x / rhs;
		temp.y = y / rhs;
		temp.z = z / rhs;
		temp.w = w / rhs;
		return temp;
	}

	inline float Vec4::operator [] (const int idx) const
	{
		RB_CORE_ASSERT(idx >= 0 && idx < 4, "Index {0} is out of bounds of Vec4", idx);
		return (&x)[idx];
	}

	inline float& Vec4::operator [] (const int idx)
	{
		RB_CORE_ASSERT(idx >= 0 && idx < 4, "Index {0} is out of bounds of Vec4", idx);
		return (&x)[idx];
	}

	inline Vec4::operator glm::vec4() const
	{
		glm::vec4 v;
		v.x = x;
		v.y = y;
		v.z = z;
		v.w = w;
		return v;
	}

	inline const Vec4& Vec4::Normalize()
	{
		float mag = GetMagnitude();
		float invMag = 1.0f / mag;
		if (0.0f * invMag == 0.0f * invMag)
		{
			x = x * invMag;
			y = y * invMag;
			z = z * invMag;
			w = w * invMag;
		}

		return *this;
	}

	inline float Vec4::GetMagnitude() const
	{
		float mag;

		mag = x * x + y * y + z * z + w * w;
		mag = sqrtf(mag);
		return mag;
	}

	inline bool Vec4::IsValid() const
	{
		if (x * 0.0f != x * 0.0f)
		{
			// x is NaN or Inf
			return false;
		}

		if (y * 0.0f != y * 0.0f)
		{
			// y is NaN or Inf
			return false;
		}

		if (z * 0.0f != z * 0.0f)
		{
			// z is NaN or Inf
			return false;
		}

		if (w * 0.0f != w * 0.0f)
		{
			// w is NaN or Inf
			return false;
		}

		return true;
	}
}
