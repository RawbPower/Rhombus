#pragma once

#include "Vector.h"

namespace rhombus::math
{
	/*
	===============================
	Mat2
	===============================
	*/
	class Mat2
	{
	public:
		Mat2() {}
		Mat2(const Mat2& rhs);
		Mat2(const float* mat);
		Mat2(const Vec2& row0, const Vec2& row1);
		Mat2& operator = (const Mat2& rhs);

		const Mat2& operator *= (const float rhs);
		const Mat2& operator += (const Mat2& rhs);

		float Determinant() const { return rows[0].x * rows[1].y - rows[0].y * rows[1].x; }

	public:
		Vec2 rows[2];
	};

	inline Mat2::Mat2(const Mat2& rhs)
	{
		rows[0] = rhs.rows[0];
		rows[1] = rhs.rows[1];
	}

	inline Mat2::Mat2(const float* mat)
	{
		rows[0] = mat + 0;
		rows[1] = mat + 2;
	}

	inline Mat2::Mat2(const Vec2& row0, const Vec2& row1)
	{
		rows[0] = row0;
		rows[1] = row1;
	}

	inline Mat2& Mat2::operator = (const Mat2& rhs)
	{
		rows[0] = rhs.rows[0];
		rows[1] = rhs.rows[1];
		return *this;
	}

	inline const Mat2& Mat2::operator *= (const float rhs)
	{
		rows[0] *= rhs;
		rows[1] *= rhs;
		return *this;
	}

	inline const Mat2& Mat2::operator += (const Mat2& rhs)
	{
		rows[0] += rhs.rows[0];
		rows[1] += rhs.rows[1];
		return *this;
	}

	/*
	===============================
	Mat3
	===============================
	*/
	class Mat3
	{
	public:
		Mat3() {}
		Mat3(const Mat3& rhs);
		Mat3(const float* mat);
		Mat3(const Vec3& row0, const Vec3& row1, const Vec3& row2);
		Mat3& operator = (const Mat3& rhs);

		void Zero();
		void Identity();

		float Trace() const;
		float Determinant() const;
		Mat3 Transpose() const;
		Mat3 Inverse() const;
		Mat2 Minor(const int i, const int j) const;
		float Cofactor(const int i, const int j) const;

		Vec3 operator * (const Vec3& rhs) const;
		Mat3 operator * (const float rhs) const;
		Mat3 operator * (const Mat3& rhs) const;
		Mat3 operator + (const Mat3& rhs) const;
		const Mat3& operator *= (const float rhs);
		const Mat3& operator += (const Mat3& rhs);

	public:
		Vec3 rows[3];
	};

	inline Mat3::Mat3(const Mat3& rhs) 
	{
		rows[0] = rhs.rows[0];
		rows[1] = rhs.rows[1];
		rows[2] = rhs.rows[2];
	}

	inline Mat3::Mat3(const float* mat) 
	{
		rows[0] = mat + 0;
		rows[1] = mat + 3;
		rows[2] = mat + 6;
	}

	inline Mat3::Mat3(const Vec3& row0, const Vec3& row1, const Vec3& row2) 
	{
		rows[0] = row0;
		rows[1] = row1;
		rows[2] = row2;
	}

	inline Mat3& Mat3::operator = (const Mat3& rhs) 
	{
		rows[0] = rhs.rows[0];
		rows[1] = rhs.rows[1];
		rows[2] = rhs.rows[2];
		return *this;
	}

	inline const Mat3& Mat3::operator *= (const float rhs) 
	{
		rows[0] *= rhs;
		rows[1] *= rhs;
		rows[2] *= rhs;
		return *this;
	}

	inline const Mat3& Mat3::operator += (const Mat3& rhs) 
	{
		rows[0] += rhs.rows[0];
		rows[1] += rhs.rows[1];
		rows[2] += rhs.rows[2];
		return *this;
	}

	inline void Mat3::Zero() 
	{
		rows[0].Zero();
		rows[1].Zero();
		rows[2].Zero();
	}

	inline void Mat3::Identity() 
	{
		rows[0] = Vec3(1, 0, 0);
		rows[1] = Vec3(0, 1, 0);
		rows[2] = Vec3(0, 0, 1);
	}

	inline float Mat3::Trace() const
	{
		return (rows[0][0] + rows[1][1] + rows[2][2]);
	}

	inline float Mat3::Determinant() const {
		const float i = rows[0][0] * (rows[1][1] * rows[2][2] - rows[1][2] * rows[2][1]);
		const float j = rows[0][1] * (rows[1][0] * rows[2][2] - rows[1][2] * rows[2][0]);
		const float k = rows[0][2] * (rows[1][0] * rows[2][1] - rows[1][1] * rows[2][0]);
		return (i - j + k);
	}

	inline Mat3 Mat3::Transpose() const 
	{
		Mat3 transpose;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				transpose.rows[i][j] = rows[j][i];
			}
		}
		return transpose;
	}

	inline Mat3 Mat3::Inverse() const 
	{
		Mat3 inv;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				inv.rows[j][i] = Cofactor(i, j);	// Perform the transpose while calculating the cofactors
			}
		}
		float det = Determinant();
		float invDet = 1.0f / det;
		inv *= invDet;
		return inv;
	}

	inline Mat2 Mat3::Minor(const int i, const int j) const 
	{
		Mat2 minor;

		int yy = 0;
		for (int y = 0; y < 3; y++) {
			if (y == j) {
				continue;
			}

			int xx = 0;
			for (int x = 0; x < 3; x++) {
				if (x == i) {
					continue;
				}

				minor.rows[xx][yy] = rows[x][y];
				xx++;
			}

			yy++;
		}
		return minor;
	}

	inline float Mat3::Cofactor(const int i, const int j) const 
	{
		const Mat2 minor = Minor(i, j);
		const float C = float(pow(-1, i + 1 + j + 1)) * minor.Determinant();
		return C;
	}

	inline Vec3 Mat3::operator * (const Vec3& rhs) const 
	{
		Vec3 tmp;
		tmp[0] = rows[0].Dot(rhs);
		tmp[1] = rows[1].Dot(rhs);
		tmp[2] = rows[2].Dot(rhs);
		return tmp;
	}

	inline Mat3 Mat3::operator * (const float rhs) const 
	{
		Mat3 tmp;
		tmp.rows[0] = rows[0] * rhs;
		tmp.rows[1] = rows[1] * rhs;
		tmp.rows[2] = rows[2] * rhs;
		return tmp;
	}

	inline Mat3 Mat3::operator * (const Mat3& rhs) const 
	{
		Mat3 tmp;
		for (int i = 0; i < 3; i++) {
			tmp.rows[i].x = rows[i].x * rhs.rows[0].x + rows[i].y * rhs.rows[1].x + rows[i].z * rhs.rows[2].x;
			tmp.rows[i].y = rows[i].x * rhs.rows[0].y + rows[i].y * rhs.rows[1].y + rows[i].z * rhs.rows[2].y;
			tmp.rows[i].z = rows[i].x * rhs.rows[0].z + rows[i].y * rhs.rows[1].z + rows[i].z * rhs.rows[2].z;
		}
		return tmp;
	}

	inline Mat3 Mat3::operator + (const Mat3& rhs) const 
	{
		Mat3 tmp;
		for (int i = 0; i < 3; i++) {
			tmp.rows[i] = rows[i] + rhs.rows[i];
		}
		return tmp;
	}

	/*
	===============================
	Mat4
	===============================
	*/
	class Mat4 
	{
	public:
		Mat4() {}
		Mat4(const Mat4& rhs);
		Mat4(const float* mat);
		Mat4(const Vec4& row0, const Vec4& row1, const Vec4& row2, const Vec4& row3);
		Mat4& operator = (const Mat4& rhs);
		~Mat4() {}

		void Zero();
		void Identity();

		float Trace() const;
		float Determinant() const;
		Mat4 Transpose() const;
		Mat4 Inverse() const;
		Mat3 Minor(const int i, const int j) const;
		float Cofactor(const int i, const int j) const;

		void Orient(Vec3 pos, Vec3 fwd, Vec3 up);
		void LookAt(Vec3 pos, Vec3 lookAt, Vec3 up);
		void LookAtOpenGL(Vec3 pos, Vec3 lookAt, Vec3 up);
		void Perspective(float fovy, float aspect_ratio, float znear, float zfar);
		void Ortho(float xmin, float xmax, float ymin, float ymax, float znear, float zfar);

		const float* ToPtr() const { return rows[0].ToPtr(); }
		float* ToPtr() { return rows[0].ToPtr(); }

		Vec4 operator * (const Vec4& rhs) const;
		Mat4 operator * (const float rhs) const;
		Mat4 operator * (const Mat4& rhs) const;
		const Mat4& operator *= (const float rhs);

	public:
		Vec4 rows[4];
	};

	inline Mat4::Mat4(const Mat4& rhs) 
	{
		rows[0] = rhs.rows[0];
		rows[1] = rhs.rows[1];
		rows[2] = rhs.rows[2];
		rows[3] = rhs.rows[3];
	}

	inline Mat4::Mat4(const float* mat) 
	{
		rows[0] = mat + 0;
		rows[1] = mat + 4;
		rows[2] = mat + 8;
		rows[3] = mat + 12;
	}

	inline Mat4::Mat4(const Vec4& row0, const Vec4& row1, const Vec4& row2, const Vec4& row3) 
	{
		rows[0] = row0;
		rows[1] = row1;
		rows[2] = row2;
		rows[3] = row3;
	}

	inline Mat4& Mat4::operator = (const Mat4& rhs) 
	{
		rows[0] = rhs.rows[0];
		rows[1] = rhs.rows[1];
		rows[2] = rhs.rows[2];
		rows[3] = rhs.rows[3];
		return *this;
	}

	inline const Mat4& Mat4::operator *= (const float rhs) 
	{
		rows[0] *= rhs;
		rows[1] *= rhs;
		rows[2] *= rhs;
		rows[3] *= rhs;
		return *this;
	}

	inline void Mat4::Zero() 
	{
		rows[0].Zero();
		rows[1].Zero();
		rows[2].Zero();
		rows[3].Zero();
	}

	inline void Mat4::Identity() 
	{
		rows[0] = Vec4(1, 0, 0, 0);
		rows[1] = Vec4(0, 1, 0, 0);
		rows[2] = Vec4(0, 0, 1, 0);
		rows[3] = Vec4(0, 0, 0, 1);
	}

	inline float Mat4::Trace() const 
	{
		return (rows[0][0] + rows[1][1] + rows[2][2] + rows[3][3]);
	}

	inline float Mat4::Determinant() const 
	{
		float det = 0.0f;
		float sign = 1.0f;
		for (int j = 0; j < 4; j++) {
			Mat3 minor = Minor(0, j);

			det += rows[0][j] * minor.Determinant() * sign;
			sign = sign * -1.0f;
		}
		return det;
	}

	inline Mat4 Mat4::Transpose() const 
	{
		Mat4 transpose;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				transpose.rows[i][j] = rows[j][i];
			}
		}
		return transpose;
	}

	inline Mat4 Mat4::Inverse() const 
	{
		Mat4 inv;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				inv.rows[j][i] = Cofactor(i, j);	// Perform the transpose while calculating the cofactors
			}
		}
		float det = Determinant();
		float invDet = 1.0f / det;
		inv *= invDet;
		return inv;
	}

	inline Mat3 Mat4::Minor(const int i, const int j) const 
	{
		Mat3 minor;

		int yy = 0;
		for (int y = 0; y < 4; y++) {
			if (y == j) {
				continue;
			}

			int xx = 0;
			for (int x = 0; x < 4; x++) {
				if (x == i) {
					continue;
				}

				minor.rows[xx][yy] = rows[x][y];
				xx++;
			}

			yy++;
		}
		return minor;
	}

	inline float Mat4::Cofactor(const int i, const int j) const 
	{
		const Mat3 minor = Minor(i, j);
		const float C = float(pow(-1, i + 1 + j + 1)) * minor.Determinant();
		return C;
	}

	inline void Mat4::Orient(Vec3 pos, Vec3 fwd, Vec3 up) 
	{
		Vec3 right = Vec3::Cross(fwd, up);

		// For coordinate system where:
		// +x-axis = right
		// +y-axis = fwd
		// +z-axis = up
		rows[0] = Vec4(right.x, fwd.x, up.x, pos.x);
		rows[1] = Vec4(right.y, fwd.y, up.y, pos.y);
		rows[2] = Vec4(right.z, fwd.z, up.z, pos.z);
		rows[3] = Vec4(0, 0, 0, 1);
	}

	inline void Mat4::LookAt(Vec3 pos, Vec3 lookAt, Vec3 up) 
	{
		Vec3 fwd = pos - lookAt;
		fwd.Normalize();

		Vec3 right = Vec3::Cross(fwd, up);
		right.Normalize();


		up = Vec3::Cross(right, fwd);
		up.Normalize();

		// For OpenGL coordinate system where:
		// +x-axis = right
		// +y-axis = fwd
		// +z-axis = up
		rows[0] = Vec4(right.x, right.y, right.z, -pos.Dot(right));
		rows[1] = Vec4(fwd.x, fwd.y, fwd.z, -pos.Dot(fwd));
		rows[2] = Vec4(up.x, up.y, up.z, -pos.Dot(up));
		rows[3] = Vec4(0, 0, 0, 1);

		// Note: OpenGL uses x: right, y: up, z: backwards
	}

	inline void Mat4::LookAtOpenGL(Vec3 pos, Vec3 lookAt, Vec3 up)
	{
		Vec3 fwd = pos - lookAt;
		fwd.Normalize();

		Vec3 right = Vec3::Cross(up, fwd);
		right.Normalize();

		up = Vec3::Cross(fwd, right);
		up.Normalize();

		// For OpenGL coordinate system where:
		// +x-axis = right
		// +y-axis = up
		// +z-axis = -fwd
		rows[0] = Vec4(right.x, right.y, right.z, -pos.Dot(right));
		rows[1] = Vec4(up.x, up.y, up.z, -pos.Dot(up));
		rows[2] = Vec4(-fwd.x, -fwd.y, -fwd.z, pos.Dot(fwd));
		rows[3] = Vec4(0, 0, 0, 1);
	}

	inline void Mat4::Perspective(float fovy, float aspect_ratio, float znear, float zfar)
	{
		// OpenGL only
		const float pi = acosf(-1.0f);
		const float fovy_radians = fovy * pi / 180.0f;
		const float f = 1.0f / tanf(fovy_radians * 0.5f);
		const float xscale = f;
		const float yscale = f / aspect_ratio;

		rows[0] = Vec4(xscale, 0, 0, 0);
		rows[1] = Vec4(0, yscale, 0, 0);
		rows[2] = Vec4(0, 0, (zfar + znear) / (znear - zfar), (2.0f * zfar * znear) / (znear - zfar));
		rows[3] = Vec4(0, 0, -1, 0);
	}
	inline void Mat4::Ortho(float xmin, float xmax, float ymin, float ymax, float znear, float zfar)
	{
		// OpenGL only
		const float width = xmax - xmin;
		const float height = ymax - ymin;
		const float depth = zfar - znear;

		const float tx = -(xmax + xmin) / width;
		const float ty = -(ymax + ymin) / height;
		const float tz = -(zfar + znear) / depth;

		rows[0] = Vec4(2.0f / width, 0, 0, tx);
		rows[1] = Vec4(0, 2.0f / height, 0, ty);
		rows[2] = Vec4(0, 0, -2.0f / depth, tz);
		rows[3] = Vec4(0, 0, 0, 1);
	}

	inline Vec4 Mat4::operator * (const Vec4& rhs) const 
	{
		Vec4 tmp;
		tmp[0] = rows[0].Dot(rhs);
		tmp[1] = rows[1].Dot(rhs);
		tmp[2] = rows[2].Dot(rhs);
		tmp[3] = rows[3].Dot(rhs);
		return tmp;
	}

	inline Mat4 Mat4::operator * (const float rhs) const 
	{
		Mat4 tmp;
		tmp.rows[0] = rows[0] * rhs;
		tmp.rows[1] = rows[1] * rhs;
		tmp.rows[2] = rows[2] * rhs;
		tmp.rows[3] = rows[3] * rhs;
		return tmp;
	}

	inline Mat4 Mat4::operator * (const Mat4& rhs) const 
	{
		Mat4 tmp;
		for (int i = 0; i < 4; i++) {
			tmp.rows[i].x = rows[i].x * rhs.rows[0].x + rows[i].y * rhs.rows[1].x + rows[i].z * rhs.rows[2].x + rows[i].w * rhs.rows[3].x;
			tmp.rows[i].y = rows[i].x * rhs.rows[0].y + rows[i].y * rhs.rows[1].y + rows[i].z * rhs.rows[2].y + rows[i].w * rhs.rows[3].y;
			tmp.rows[i].z = rows[i].x * rhs.rows[0].z + rows[i].y * rhs.rows[1].z + rows[i].z * rhs.rows[2].z + rows[i].w * rhs.rows[3].z;
			tmp.rows[i].w = rows[i].x * rhs.rows[0].w + rows[i].y * rhs.rows[1].w + rows[i].z * rhs.rows[2].w + rows[i].w * rhs.rows[3].w;
		}
		return tmp;
	}

}
