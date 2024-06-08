#include "rbpch.h"
#include "Math.h"

namespace rhombus::math
{
	static const float EPSILON_FLOAT = 1.0e-4F;

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