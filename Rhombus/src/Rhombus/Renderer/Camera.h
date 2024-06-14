#pragma once

#include "Rhombus/Math/Matrix.h"

namespace rhombus
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(const Mat4& projection)
			: m_projection(projection) {}

		virtual ~Camera() = default;

		const Mat4& GetProjection() const
		{ 
			return m_projection;
		}

	protected:
		Mat4 m_projection = Mat4::Identity();
	};
}
