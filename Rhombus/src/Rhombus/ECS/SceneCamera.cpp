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
		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;

		RecalculateProjection();
	}

	void SceneCamera::SetViewportResize(uint32_t width, uint32_t height)
	{
		m_AspectRaio = (float)width / (float)height;

		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		float orthoLeft = -m_AspectRaio * m_OrthographicSize * 0.5f;
		float orthoRight = m_AspectRaio * m_OrthographicSize * 0.5f;
		float orthoBottom = -m_OrthographicSize * 0.5f;
		float orthoTop = m_OrthographicSize * 0.5f;
		m_projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
	}
}