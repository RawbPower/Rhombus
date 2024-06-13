#include "rbpch.h"
#include "Math.h"

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/compatibility.hpp>

namespace rhombus::math
{
	static const float EPSILON_FLOAT = 1.0e-4F;

	float Lerp(float x, float y, float a)
	{
		return glm::lerp(x, y, a);
	}

	Vec2 Lerp(const Vec2& x, const Vec2& y, const Vec2& a)
	{
		glm::vec2 xGLM = glm::vec2(x.x, x.y);
		glm::vec2 yGLM = glm::vec2(y.x, y.y);
		glm::vec2 aGLM = glm::vec2(a.x, a.y);
		glm::vec2 resultGLM = glm::lerp(xGLM, yGLM, aGLM);
		return Vec2(resultGLM.x, resultGLM.y);
	}

	Vec3 Lerp(const Vec3& x, const Vec3& y, const Vec3& a)
	{
		glm::vec3 xGLM = glm::vec3(x.x, x.y, x.z);
		glm::vec3 yGLM = glm::vec3(y.x, y.y, y.z);
		glm::vec3 aGLM = glm::vec3(a.x, a.y, a.z);
		glm::vec3 resultGLM = glm::lerp(xGLM, yGLM, aGLM);
		return Vec3(resultGLM.x, resultGLM.y, resultGLM.z);
	}

	Vec4 Lerp(const Vec4& x, const Vec4& y, const Vec4& a)
	{
		glm::quat(glm::vec3(1.0f, 1.0f, 1.0f));
		glm::vec4 xGLM = glm::vec4(x.x, x.y, x.z, x.w);
		glm::vec4 yGLM = glm::vec4(y.x, y.y, y.z, y.w);
		glm::vec4 aGLM = glm::vec4(a.x, a.y, a.z, a.w);
		glm::vec4 resultGLM = glm::lerp(xGLM, yGLM, aGLM);
		return Vec4(resultGLM.x, resultGLM.y, resultGLM.z, resultGLM.w);
	}

	Vec3 Rotate(const Quat& rotation, const Vec3& axis)
	{
		// TODO remake this, leaving as glm for now to reduce functional changes
		glm::quat rotationGLM = glm::quat(rotation.w, rotation.x, rotation.y, rotation.z);
		glm::vec3 axisGLM = glm::vec3(axis.x, axis.y, axis.z);
		glm::vec3 resultGLM = glm::rotate(rotationGLM, axisGLM);
		return Vec3(resultGLM.x, resultGLM.y, resultGLM.z);
	}

	Mat4 Translate(const Mat4& mat, const Vec3& translation)
	{
		Mat4 result(mat);
		result[3] = mat[0] * translation[0] + mat[1] * translation[1] + mat[2] * translation[2] + mat[3];
		return result;
	}

	Mat4 Rotate(const Mat4& mat, float angle, const Vec3& v)
	{
		const float a = angle;
		const float c = cos(a);
		const float s = sin(a);

		Vec3 axis = Vec3::Normalize(v);
		Vec3 temp((1.0f - c) * axis);

		Mat4 rotate;
		rotate[0][0] = c + temp[0] * axis[0];
		rotate[0][1] = temp[0] * axis[1] + s * axis[2];
		rotate[0][2] = temp[0] * axis[2] - s * axis[1];

		rotate[1][0] = temp[1] * axis[0] - s * axis[2];
		rotate[1][1] = c + temp[1] * axis[1];
		rotate[1][2] = temp[1] * axis[2] + s * axis[0];

		rotate[2][0] = temp[2] * axis[0] + s * axis[1];
		rotate[2][1] = temp[2] * axis[1] - s * axis[0];
		rotate[2][2] = c + temp[2] * axis[2];

		Mat4 result;
		result[0] = mat[0] * rotate[0][0] + mat[1] * rotate[0][1] + mat[2] * rotate[0][2];
		result[1] = mat[0] * rotate[1][0] + mat[1] * rotate[1][1] + mat[2] * rotate[1][2];
		result[2] = mat[0] * rotate[2][0] + mat[1] * rotate[2][1] + mat[2] * rotate[2][2];
		result[3] = mat[3];
		return result;
	}

	Mat4 Scale(const Mat4& mat, const Vec3& scale)
	{
		Mat4 result;
		result[0] = mat[0] * scale[0];
		result[1] = mat[1] * scale[1];
		result[2] = mat[2] * scale[2];
		result[3] = mat[3];
		return result;
	}

	bool EpsilonEqual(float x, float y, float epsilon = EPSILON_FLOAT)
	{
		return abs(x - y) < epsilon;
	}

	bool EpsilonNotEqual(float x, float y, float epsilon = EPSILON_FLOAT)
	{
		return !EpsilonEqual(x,y,epsilon);
	}

	bool DecomposeTransform(const Mat4& transform, Vec3& translation, Vec3& rotation, Vec3& scale)
	{
		// Simplified from glm::decompose in matrix_decompose.inl

		Mat4 LocalMatrix(transform);

		// Normalize the matrix.
		if (EpsilonEqual(LocalMatrix[3][3], 0.0f))
			return false;

		// First, isolate perspective.  This is the messiest.
		if (
			EpsilonNotEqual(LocalMatrix[0][3], 0.0f) ||
			EpsilonNotEqual(LocalMatrix[1][3], 0.0f) ||
			EpsilonNotEqual(LocalMatrix[2][3], 0.0f))
		{
			// Clear the perspective partition
			LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = 0.0f;
			LocalMatrix[3][3] = 1.0f;
		}

		// Next take care of translation (easy).
		Vec4 tran4 = LocalMatrix[3];
		translation = Vec3(tran4);
		LocalMatrix[3] = Vec4(0, 0, 0, LocalMatrix[3].w);

		Vec3 Row[3], Pdum3;

		// Now get scale and shear.
		for (size_t i = 0; i < 3; ++i)
			for (size_t j = 0; j < 3; ++j)
				Row[i][j] = LocalMatrix[i][j];

		// Compute X scale factor and normalize first row.
		scale.x = Row[0].GetMagnitude();
		scale.y = Row[1].GetMagnitude();
		scale.z = Row[2].GetMagnitude();
#if 0
		// At this point, the matrix (in rows[]) is orthonormal.
		// Check for a coordinate system flip.  If the determinant
		// is -1, then negate the matrix and the scaling factors.
		Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
		if (dot(Row[0], Pdum3) < 0)
		{
			for (length_t i = 0; i < 3; i++)
			{
				scale[i] *= static_cast<T>(-1);
				Row[i] *= static_cast<T>(-1);
			}
		}
#endif

		rotation.y = asin(-Row[0][2]);
		if (cos(rotation.y) != 0) {
			rotation.x = atan2(Row[1][2], Row[2][2]);
			rotation.z = atan2(Row[0][1], Row[0][0]);
		}
		else {
			rotation.x = atan2(-Row[2][0], Row[1][1]);
			rotation.z = 0;
		}


		return true;
	}
}