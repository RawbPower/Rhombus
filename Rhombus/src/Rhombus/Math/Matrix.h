#pragma once

#include "Vector.h"

namespace rhombus
{
	// TODO: make function types consistent in form
	// ie. Inverse(mat) = mat (new matrix)
	// mat.Inverse() = &mat (same matrix)

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
		Mat2(const Vec2& col0, const Vec2& col1);
		Mat2& operator = (const Mat2& rhs);

		const Mat2& operator *= (const float rhs);
		const Mat2& operator += (const Mat2& rhs);

		float Determinant() const { return cols[0][0] * cols[1][1] - cols[0][1] * cols[1][0]; }

		const float* ToPtr() const { return cols[0].ToPtr(); }
		float* ToPtr() { return cols[0].ToPtr(); }

	public:
		Vec2 cols[2];
	};

	inline Mat2::Mat2(const Mat2& rhs)
	{
		cols[0] = rhs.cols[0];
		cols[1] = rhs.cols[1];
	}

	inline Mat2::Mat2(const float* mat)
	{
		cols[0] = mat + 0;
		cols[1] = mat + 2;
	}

	inline Mat2::Mat2(const Vec2& col0, const Vec2& col1)
	{
		cols[0] = col0;
		cols[1] = col1;
	}

	inline Mat2& Mat2::operator = (const Mat2& rhs)
	{
		cols[0] = rhs.cols[0];
		cols[1] = rhs.cols[1];
		return *this;
	}

	inline const Mat2& Mat2::operator *= (const float rhs)
	{
		cols[0] *= rhs;
		cols[1] *= rhs;
		return *this;
	}

	inline const Mat2& Mat2::operator += (const Mat2& rhs)
	{
		cols[0] += rhs.cols[0];
		cols[1] += rhs.cols[1];
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
		Mat3(const Vec3& col0, const Vec3& col1, const Vec3& col2);
		Mat3& operator = (const Mat3& rhs);

		void Zero();
		void Identity();

		float Trace() const;
		float Determinant() const;
		Mat3 Transpose() const;
		Mat3 Inverse() const;
		Mat2 Minor(const int c, const int r) const;
		float Cofactor(const int c, const int r) const;

		Vec3 operator * (const Vec3& rhs) const;
		Mat3 operator * (const float rhs) const;
		Mat3 operator * (const Mat3& rhs) const;
		Mat3 operator + (const Mat3& rhs) const;
		const Mat3& operator *= (const float rhs);
		const Mat3& operator += (const Mat3& rhs);

		const float* ToPtr() const { return cols[0].ToPtr(); }
		float* ToPtr() { return cols[0].ToPtr(); }

	public:
		Vec3 cols[3];
	};

	inline Mat3::Mat3(const Mat3& rhs) 
	{
		cols[0] = rhs.cols[0];
		cols[1] = rhs.cols[1];
		cols[2] = rhs.cols[2];
	}

	inline Mat3::Mat3(const float* mat) 
	{
		cols[0] = mat + 0;
		cols[1] = mat + 3;
		cols[2] = mat + 6;
	}

	inline Mat3::Mat3(const Vec3& col0, const Vec3& col1, const Vec3& col2) 
	{
		cols[0] = col0;
		cols[1] = col1;
		cols[2] = col2;
	}

	inline Mat3& Mat3::operator = (const Mat3& rhs) 
	{
		cols[0] = rhs.cols[0];
		cols[1] = rhs.cols[1];
		cols[2] = rhs.cols[2];
		return *this;
	}

	inline const Mat3& Mat3::operator *= (const float rhs) 
	{
		cols[0] *= rhs;
		cols[1] *= rhs;
		cols[2] *= rhs;
		return *this;
	}

	inline const Mat3& Mat3::operator += (const Mat3& rhs) 
	{
		cols[0] += rhs.cols[0];
		cols[1] += rhs.cols[1];
		cols[2] += rhs.cols[2];
		return *this;
	}

	inline void Mat3::Zero() 
	{
		cols[0].Zero();
		cols[1].Zero();
		cols[2].Zero();
	}

	inline void Mat3::Identity() 
	{
		cols[0] = Vec3(1, 0, 0);
		cols[1] = Vec3(0, 1, 0);
		cols[2] = Vec3(0, 0, 1);
	}

	inline float Mat3::Trace() const
	{
		return (cols[0][0] + cols[1][1] + cols[2][2]);
	}

	inline float Mat3::Determinant() const {
		const float i = cols[0][0] * (cols[1][1] * cols[2][2] - cols[2][1] * cols[1][2]);
		const float j = cols[1][0] * (cols[0][1] * cols[2][2] - cols[2][1] * cols[0][2]);
		const float k = cols[2][0] * (cols[0][1] * cols[1][2] - cols[1][1] * cols[0][2]);
		return (i - j + k);
	}

	inline Mat3 Mat3::Transpose() const 
	{
		Mat3 transpose;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				transpose.cols[i][j] = cols[j][i];
			}
		}
		return transpose;
	}

	// Check this!
	inline Mat3 Mat3::Inverse() const 
	{
		Mat3 inv;
		for (int c = 0; c < 3; c++) {
			for (int r = 0; r < 3; r++) {
				inv.cols[c][r]= Cofactor(c, r);	// Perform the transpose while calculating the cofactors
			}
		}
		float det = Determinant();
		float invDet = 1.0f / det;
		inv *= invDet;
		return inv;
	}

	inline Mat2 Mat3::Minor(const int c, const int r) const 
	{
		Mat2 minor;

		int xx = 0;
		for (int x = 0; x < 3; x++) {
			if (x == c) {
				continue;
			}

			int yy = 0;
			for (int y = 0; y < 3; y++) {
				if (y == r) {
					continue;
				}

				minor.cols[yy][xx] = cols[y][x];
				yy++;
			}

			xx++;
		}
		return minor;
	}

	inline float Mat3::Cofactor(const int c, const int r) const 
	{
		const Mat2 minor = Minor(c, r);
		const float C = float(pow(-1, c + 1 + r + 1)) * minor.Determinant();
		return C;
	}

	inline Vec3 Mat3::operator * (const Vec3& rhs) const
	{
		Vec3 tmp;
		tmp[0] = cols[0][0] * rhs.x + cols[1][0] * rhs.y + cols[2][0] * rhs.z;
		tmp[1] = cols[0][1] * rhs.x + cols[1][1] * rhs.y + cols[2][1] * rhs.z;
		tmp[2] = cols[0][2] * rhs.x + cols[1][2] * rhs.y + cols[2][2] * rhs.z;
		return tmp;
	}

	inline Mat3 Mat3::operator * (const float rhs) const 
	{
		Mat3 tmp;
		tmp.cols[0] = cols[0] * rhs;
		tmp.cols[1] = cols[1] * rhs;
		tmp.cols[2] = cols[2] * rhs;
		return tmp;
	}

	inline Mat3 Mat3::operator * (const Mat3& rhs) const 
	{
		Mat3 tmp;
		for (int i = 0; i < 3; i++) 
		{
			tmp.cols[i][0] = cols[0][0] * rhs.cols[i][0] + cols[1][0] * rhs.cols[i][1] + cols[2][0] * rhs.cols[i][2];
			tmp.cols[i][1] = cols[0][1] * rhs.cols[i][0] + cols[1][1] * rhs.cols[i][1] + cols[2][1] * rhs.cols[i][2];
			tmp.cols[i][2] = cols[0][2] * rhs.cols[i][0] + cols[1][2] * rhs.cols[i][1] + cols[2][2] * rhs.cols[i][2];
		}
		return tmp;
	}

	inline Mat3 Mat3::operator + (const Mat3& rhs) const 
	{
		Mat3 tmp;
		for (int i = 0; i < 3; i++) 
		{
			tmp.cols[i] = cols[i] + rhs.cols[i];
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
		Mat4(const Vec4& col0, const Vec4& col1, const Vec4& col2, const Vec4& col3);
		Mat4& operator = (const Mat4& rhs);
		Vec4 operator [] (const int idx) const;
		Vec4& operator [] (const int idx);
		~Mat4() {}

		Vec3 a() { return cols[0].GetXYZ(); }
		Vec3 b() { return cols[1].GetXYZ(); }
		Vec3 c() { return cols[2].GetXYZ(); }
		Vec3 d() { return cols[3].GetXYZ(); }

		static Mat4 Zero();
		static Mat4 Identity();

		float Trace() const;
		float Determinant() const;
		Mat4 Transpose() const;
		Mat4 Inverse() const;
		Mat3 Minor(const int c, const int r) const;
		float Cofactor(const int c, const int r) const;

		/*void Orient(Vec3 pos, Vec3 fwd, Vec3 up);
		void LookAt(Vec3 pos, Vec3 lookAt, Vec3 up);
		void LookAtOpenGL(Vec3 pos, Vec3 lookAt, Vec3 up);*/
		static Mat4 Perspective(float fovy, float aspect_ratio, float znear, float zfar);
		static Mat4 Ortho(float xmin, float xmax, float ymin, float ymax, float znear, float zfar);

		const float* ToPtr() const { return cols[0].ToPtr(); }
		float* ToPtr() { return cols[0].ToPtr(); }

		Vec4 operator * (const Vec4& rhs) const;
		Mat4 operator * (const float rhs) const;
		Mat4 operator * (const Mat4& rhs) const;
		const Mat4& operator *= (const float rhs);

	public:
		Vec4 cols[4];
	};

	inline Mat4::Mat4(const Mat4& rhs) 
	{
		cols[0] = rhs.cols[0];
		cols[1] = rhs.cols[1];
		cols[2] = rhs.cols[2];
		cols[3] = rhs.cols[3];
	}

	inline Mat4::Mat4(const float* mat) 
	{
		cols[0] = mat + 0;
		cols[1] = mat + 4;
		cols[2] = mat + 8;
		cols[3] = mat + 12;
	}

	inline Mat4::Mat4(const Vec4& col0, const Vec4& col1, const Vec4& col2, const Vec4& col3) 
	{
		cols[0] = col0;
		cols[1] = col1;
		cols[2] = col2;
		cols[3] = col3;
	}

	inline Mat4& Mat4::operator = (const Mat4& rhs) 
	{
		cols[0] = rhs.cols[0];
		cols[1] = rhs.cols[1];
		cols[2] = rhs.cols[2];
		cols[3] = rhs.cols[3];
		return *this;
	}

	inline const Mat4& Mat4::operator *= (const float rhs) 
	{
		cols[0] *= rhs;
		cols[1] *= rhs;
		cols[2] *= rhs;
		cols[3] *= rhs;
		return *this;
	}

	inline Vec4 Mat4::operator [] (const int idx) const
	{
		return cols[idx];
	}

	inline Vec4& Mat4::operator [] (const int idx)
	{
		return cols[idx];
	}

	inline Mat4 Mat4::Zero() 
	{
		return Mat4({ 0.0f, 0.0f, 0.0f, 0.0f },
					{ 0.0f, 0.0f, 0.0f, 0.0f }, 
					{ 0.0f, 0.0f, 0.0f, 0.0f }, 
					{ 0.0f, 0.0f, 0.0f, 0.0f });
	}

	inline Mat4 Mat4::Identity() 
	{
		return Mat4({ 1.0f, 0.0f, 0.0f, 0.0f },
					{ 0.0f, 1.0f, 0.0f, 0.0f },
					{ 0.0f, 0.0f, 1.0f, 0.0f },
					{ 0.0f, 0.0f, 0.0f, 1.0f });
	}

	inline float Mat4::Trace() const 
	{
		return (cols[0][0] + cols[1][1] + cols[2][2] + cols[3][3]);
	}

	inline float Mat4::Determinant() const 
	{
		float det = 0.0f;
		float sign = 1.0f;
		for (int j = 0; j < 4; j++) 
		{
			Mat3 minor = Minor(0, j);

			det += cols[j][0] * minor.Determinant() * sign;
			sign = sign * -1.0f;
		}
		return det;
	}

	inline Mat4 Mat4::Transpose() const 
	{
		Mat4 transpose;
		for (int i = 0; i < 4; i++) 
		{
			for (int j = 0; j < 4; j++) 
			{
				transpose.cols[i][j] = cols[j][i];
			}
		}
		return transpose;
	}

	// Check this!
	inline Mat4 Mat4::Inverse() const 
	{
		Mat4 inv;
		for (int c = 0; c < 4; c++) 
		{
			for (int r = 0; r < 4; r++) 
			{
				inv.cols[c][r] = Cofactor(c, r);	// Perform the transpose while calculating the cofactors
			}
		}
		float det = Determinant();
		float invDet = 1.0f / det;
		inv *= invDet;

		return inv;
	}

	inline Mat3 Mat4::Minor(const int c, const int r) const 
	{
		Mat3 minor;

		int xx = 0;
		for (int x = 0; x < 4; x++) 
		{
			if (x == c) 
			{
				continue;
			}

			int yy = 0;
			for (int y = 0; y < 4; y++) 
			{
				if (y == r) {
					continue;
				}

				minor.cols[yy][xx] = cols[y][x];
				yy++;
			}

			xx++;
		}
		return minor;
	}

	inline float Mat4::Cofactor(const int c, const int r) const 
	{
		const Mat3 minor = Minor(c, r);
		const float C = float(pow(-1, c + 1 + r + 1)) * minor.Determinant();
		return C;
	}

	/*inline void Mat4::Orient(Vec3 pos, Vec3 fwd, Vec3 up)
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
	}*/

	// Check this!
	inline Mat4 Mat4::Perspective(float fovy, float aspect_ratio, float znear, float zfar)
	{
		// OpenGL only
		const float f = 1.0f / tanf(fovy * 0.5f);
		const float xscale = f / aspect_ratio;
		const float yscale = f;
		
		return Mat4({xscale, 0.0f, 0.0f, 0.0f},
					{ 0.0f, yscale, 0.0f, 0.0f },
					{ 0.0f, 0.0f, (zfar + znear) / (znear - zfar), -1.0f },
					{ 0.0f, 0.0f, (2.0f * zfar * znear) / (znear - zfar), 0.0f } );
	}

	// Check this!
	inline Mat4 Mat4::Ortho(float xmin, float xmax, float ymin, float ymax, float znear, float zfar)
	{
		// OpenGL only
		const float width = xmax - xmin;
		const float height = ymax - ymin;
		const float depth = zfar - znear;

		const float tx = -(xmax + xmin) / width;
		const float ty = -(ymax + ymin) / height;
		const float tz = -(zfar + znear) / depth;

		return Mat4({2.0f / width, 0.0f, 0.0f, 0.0f},
			{ 0.0f, 2.0f / height, 0.0f, 0.0f },
			{ 0.0f, 0.0f, -2.0f / depth, 0.0f },
			{ tx, ty, tz, 1.0f });
	}

	inline Vec4 Mat4::operator * (const Vec4& rhs) const 
	{
		return Vec4(cols[0][0] * rhs[0] + cols[1][0] * rhs[1] + cols[2][0] * rhs[2] + cols[3][0] * rhs[3],
			cols[0][1] * rhs[0] + cols[1][1] * rhs[1] + cols[2][1] * rhs[2] + cols[3][1] * rhs[3],
			cols[0][2] * rhs[0] + cols[1][2] * rhs[1] + cols[2][2] * rhs[2] + cols[3][2] * rhs[3],
			cols[0][3] * rhs[0] + cols[1][3] * rhs[1] + cols[2][3] * rhs[2] + cols[3][3] * rhs[3]);
	}

	inline Mat4 Mat4::operator * (const float rhs) const 
	{
		Mat4 tmp;
		tmp.cols[0] = cols[0] * rhs;
		tmp.cols[1] = cols[1] * rhs;
		tmp.cols[2] = cols[2] * rhs;
		tmp.cols[3] = cols[3] * rhs;
		return tmp;
	}

	inline Mat4 Mat4::operator * (const Mat4& rhs) const 
	{
		Mat4 tmp;
		for (int i = 0; i < 4; i++)
		{
			tmp.cols[i][0] = cols[0][0] * rhs.cols[i][0] + cols[1][0] * rhs.cols[i][1] + cols[2][0] * rhs.cols[i][2] + cols[3][0] * rhs.cols[i][3];
			tmp.cols[i][1] = cols[0][1] * rhs.cols[i][0] + cols[1][1] * rhs.cols[i][1] + cols[2][1] * rhs.cols[i][2] + cols[3][1] * rhs.cols[i][3];
			tmp.cols[i][2] = cols[0][2] * rhs.cols[i][0] + cols[1][2] * rhs.cols[i][1] + cols[2][2] * rhs.cols[i][2] + cols[3][2] * rhs.cols[i][3];
			tmp.cols[i][3] = cols[0][3] * rhs.cols[i][0] + cols[1][3] * rhs.cols[i][1] + cols[2][3] * rhs.cols[i][2] + cols[3][3] * rhs.cols[i][3];
		}
		return tmp;
	}

}
