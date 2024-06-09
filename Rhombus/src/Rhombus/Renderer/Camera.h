#pragma once

#include "Rhombus/Math/Matrix.h"

#include <glm/glm.hpp>

namespace rhombus
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(const Mat4& projection)
			: m_projection(projection) {}
		Camera(const glm::mat4& projection)
		{
			Vec4 row0 = Vec4(projection[0][0], projection[0][1], projection[0][2], projection[0][3]);
			Vec4 row1 = Vec4(projection[1][0], projection[1][1], projection[1][2], projection[1][3]);
			Vec4 row2 = Vec4(projection[2][0], projection[2][1], projection[2][2], projection[2][3]);
			Vec4 row3 = Vec4(projection[3][0], projection[3][1], projection[3][2], projection[3][3]);
			m_projection = Mat4(row0, row1, row2, row3);
		}

		virtual ~Camera() = default;

		//const glm::mat4& GetProjection() const { return m_projection; }
		const Mat4& GetProjection() const
		{ 
			return m_projection;
		}

	protected:
		Mat4 m_projection = Mat4::Identity();
	};
}
