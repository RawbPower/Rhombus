#pragma once

#include "Rhombus/Math/Matrix.h"

namespace rhombus {

	class OrthographicCamera {
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		void SetProjection(float left, float right, float bottom, float top);

		const Vec3& GetPosition() const { return m_Position; }
		void SetPosition(const Vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		float GetRotation() const { return m_Rotation; }
		void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

		const Mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const Mat4 GetViewMatrix() const { return m_ViewMatrix; }
		const Mat4 GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
	private:
		void RecalculateViewMatrix();
	private:
		Mat4 m_ProjectionMatrix;
		Mat4 m_ViewMatrix;
		Mat4 m_ViewProjectionMatrix;

		Vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;
	};
}
