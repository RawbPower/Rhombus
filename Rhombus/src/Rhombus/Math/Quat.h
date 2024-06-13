#pragma once

#include "Vector.h"
#include "Matrix.h"

namespace rhombus
{
	/*
	===============================
	Quat
	===============================
	*/
	class Quat 
	{
	public:
		Quat();
		Quat(const Quat& rhs);
		Quat(float X, float Y, float Z, float W);
		Quat(Vec3 v);
		Quat(Vec3 n, const float angleRadians);
		const Quat& operator = (const Quat& rhs);

		Quat& operator *= (const float& rhs);
		Quat& operator *= (const Quat& rhs);
		Quat operator * (const Quat& rhs) const;

		void	Normalize();
		void	Invert();
		Quat	Inverse() const;
		float	Mag2() const;
		float	GetMagnitude() const;
		Vec3	RotatePoint(const Vec3& rhs) const;
		Mat3	RotateMatrix(const Mat3& rhs) const;
		Vec3	xyz() const { return Vec3(x, y, z); }
		bool	IsValid() const;

		Mat3	ToMat3() const;
		Mat4	ToMat4() const;
		Vec4	ToVec4() const { return Vec4(w, x, y, z); }

		Vec3    GetNormal() const { return GetAngle() > 0.0f ? Vec3(x, y, z) / sinf(GetAngle() / 2.0f) : Vec3(1, 0, 0); }
		float   GetAngle() const { return acos(w) * 2.0f; }

	public:
		float w;
		float x;
		float y;
		float z;
	};

	inline Quat::Quat() :
		x(0),
		y(0),
		z(0),
		w(1) 
	{
	}

	inline Quat::Quat(const Quat& rhs) :
		x(rhs.x),
		y(rhs.y),
		z(rhs.z),
		w(rhs.w) 
	{
	}

	inline Quat::Quat(float X, float Y, float Z, float W) :
		x(X),
		y(Y),
		z(Z),
		w(W) 
	{
	}

	inline Quat::Quat(Vec3 v)
	{
		Vec3 c = Vec3(cos(v.x * 0.5f), cos(v.y * 0.5f), cos(v.z * 0.5f));
		Vec3 s = Vec3(sin(v.x * 0.5f), sin(v.y * 0.5f), sin(v.z * 0.5f));

		this->w = c.x * c.y * c.z + s.x * s.y * s.z;
		this->x = s.x * c.y * c.z - c.x * s.y * s.z;
		this->y = c.x * s.y * c.z + s.x * c.y * s.z;
		this->z = c.x * c.y * s.z - s.x * s.y * c.z;
	}

	inline Quat::Quat(Vec3 n, const float angleRadians) 
	{
		const float halfAngleRadians = 0.5f * angleRadians;

		w = cosf(halfAngleRadians);

		const float halfSine = sinf(halfAngleRadians);
		n.Normalize();
		x = n.x * halfSine;
		y = n.y * halfSine;
		z = n.z * halfSine;
	}

	inline const Quat& Quat::operator = (const Quat& rhs) 
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;
		return *this;
	}

	inline Quat& Quat::operator *= (const float& rhs) 
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		w *= rhs;
		return *this;
	}

	inline Quat& Quat::operator *= (const Quat& rhs) 
	{
		Quat temp = *this * rhs;
		w = temp.w;
		x = temp.x;
		y = temp.y;
		z = temp.z;
		return *this;
	}

	// Quaternion Rules
	// i*i = j*j = k*k = -1
	// i = j*k = -k*j
	// j = k*i = -i*k 
	// k = i*j = -j*i
	// 
	// Multiplication
	// q = w + v = w + x*i + y*j + z*k
	// q1*q2 = (w1*w2 - Dot(v1,v2), w1 * v2 + w2 * v1 + Cross(v1,v2)
	inline Quat Quat::operator * (const Quat& rhs) const 
	{
		Quat temp;
		temp.w = (w * rhs.w) - (x * rhs.x) - (y * rhs.y) - (z * rhs.z);
		temp.x = (x * rhs.w) + (w * rhs.x) + (y * rhs.z) - (z * rhs.y);
		temp.y = (y * rhs.w) + (w * rhs.y) + (z * rhs.x) - (x * rhs.z);
		temp.z = (z * rhs.w) + (w * rhs.z) + (x * rhs.y) - (y * rhs.x);
		return temp;
	}

	inline void Quat::Normalize() 
	{
		float invMag = 1.0f / GetMagnitude();

		if (0.0f * invMag == 0.0f * invMag) 
		{
			x = x * invMag;
			y = y * invMag;
			z = z * invMag;
			w = w * invMag;
		}
	}

	inline void Quat::Invert() 
	{
		*this *= 1.0f / Mag2();
		x = -x;
		y = -y;
		z = -z;
	}

	inline Quat Quat::Inverse() const 
	{
		Quat val(*this);
		val.Invert();
		return val;
	}

	inline float Quat::Mag2() const 
	{
		return ((x * x) + (y * y) + (z * z) + (w * w));
	}

	inline float Quat::GetMagnitude() const 
	{
		return sqrtf(Mag2());
	}

	inline Vec3 Quat::RotatePoint(const Vec3& rhs) const 
	{
		Quat vector(rhs.x, rhs.y, rhs.z, 0.0f);
		Quat final = *this * vector * Inverse();
		return Vec3(final.x, final.y, final.z);
	}

	inline bool Quat::IsValid() const 
	{
		if (x * 0 != x * 0) {
			return false;
		}
		if (y * 0 != y * 0) {
			return false;
		}
		if (z * 0 != z * 0) {
			return false;
		}
		if (w * 0 != w * 0) {
			return false;
		}
		return true;
	}

	// Check this!
	inline Mat3 Quat::RotateMatrix(const Mat3& rhs) const 
	{
		Mat3 mat;
		mat.cols[0] = RotatePoint(rhs.cols[0]);
		mat.cols[1] = RotatePoint(rhs.cols[1]);
		mat.cols[2] = RotatePoint(rhs.cols[2]);
		return mat;
	}

	// Check this!
	inline Mat3 Quat::ToMat3() const 
	{
		Mat3 mat;
		mat.Identity();

		mat.cols[0] = RotatePoint(mat.cols[0]);
		mat.cols[1] = RotatePoint(mat.cols[1]);
		mat.cols[2] = RotatePoint(mat.cols[2]);
		return mat;
	}

	// Check this!
	inline Mat4 Quat::ToMat4() const
	{
		Mat4 mat = Mat4::Identity();

		mat.cols[0] = Vec4(RotatePoint(mat.cols[0]), 0.0f);
		mat.cols[1] = Vec4(RotatePoint(mat.cols[1]), 0.0f);
		mat.cols[2] = Vec4(RotatePoint(mat.cols[2]), 0.0f);
		mat.cols[3] = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
		return mat;
	}
}