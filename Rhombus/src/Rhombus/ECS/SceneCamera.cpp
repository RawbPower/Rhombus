#include "rbpch.h"
#include "SceneCamera.h"

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
			glm::mat4 projection = Mat4::Ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
			m_projection = projection;
		}
		else
		{
			glm::mat4 projection = Mat4::Perspective(m_PerspectiveFOV, m_AspectRaio, m_PerspectiveNear, m_PerspectiveFar);
			m_projection = projection;
		}
	}
}