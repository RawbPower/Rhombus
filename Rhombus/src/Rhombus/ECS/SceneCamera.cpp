#include "rbpch.h"
#include "SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace rhombus
{

	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_projectionType == ProjectionType::Orthographic;

		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;

		RecalculateProjection();
	}

	void SceneCamera::SetPerspective(float verticalFOV, float nearClip, float farClip)
	{
		m_projectionType == ProjectionType::Perspective;

		m_PerspectiveFOV = verticalFOV;
		m_PerspectiveNear = nearClip;
		m_PerspectiveFar = farClip;

		RecalculateProjection();
	}

	void SceneCamera::SetViewportResize(uint32_t width, uint32_t height)
	{
		m_AspectRaio = (float)width / (float)height;

		m_PixelPerfectSize = (float)height;

		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		if (m_projectionType == ProjectionType::Orthographic)
		{
			if (m_PixelPerfect)
			{
				m_OrthographicSize = m_PixelPerfectSize;
			}

			float orthoLeft = -m_AspectRaio * m_OrthographicSize * 0.5f;
			float orthoRight = m_AspectRaio * m_OrthographicSize * 0.5f;
			float orthoBottom = -m_OrthographicSize * 0.5f;
			float orthoTop = m_OrthographicSize * 0.5f;
			glm::mat4 projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
			Vec4 row0 = Vec4(projection[0][0], projection[0][1], projection[0][2], projection[0][3]);
			Vec4 row1 = Vec4(projection[1][0], projection[1][1], projection[1][2], projection[1][3]);
			Vec4 row2 = Vec4(projection[2][0], projection[2][1], projection[2][2], projection[2][3]);
			Vec4 row3 = Vec4(projection[3][0], projection[3][1], projection[3][2], projection[3][3]);
			m_projection = Mat4(row0, row1, row2, row3);
		}
		else
		{
			glm::mat4 projection = glm::perspective(m_PerspectiveFOV, m_AspectRaio, m_PerspectiveNear, m_PerspectiveFar);
			Vec4 row0 = Vec4(projection[0][0], projection[0][1], projection[0][2], projection[0][3]);
			Vec4 row1 = Vec4(projection[1][0], projection[1][1], projection[1][2], projection[1][3]);
			Vec4 row2 = Vec4(projection[2][0], projection[2][1], projection[2][2], projection[2][3]);
			Vec4 row3 = Vec4(projection[3][0], projection[3][1], projection[3][2], projection[3][3]);
			m_projection = Mat4(row0, row1, row2, row3);
		}
	}
}