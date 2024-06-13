#include "rbpch.h"
#include "OrthographicCamera.h"

#include "Rhombus/Math/Vector.h"
#include "Rhombus/Math/Matrix.h"
#include "Rhombus/Math/Math.h"

namespace rhombus {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_ProjectionMatrix(Mat4::Ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(Mat4::Identity())
	{
		RB_PROFILE_FUNCTION();

		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		RB_PROFILE_FUNCTION();

		m_ProjectionMatrix = Mat4::Ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		RB_PROFILE_FUNCTION();

		// Transform matrix of the camera itself
		Mat4 transform = math::Translate(Mat4::Identity(), m_Position) *
			math::Rotate(Mat4::Identity(), m_Rotation * math::DegToRad, Vec3(0, 0, 1));

		// To get the ViewMatrix (ie how objects in the scene transfrom with the camera) we simple get the inverse
		m_ViewMatrix = transform.Inverse();
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}