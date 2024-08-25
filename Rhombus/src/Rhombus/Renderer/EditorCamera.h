#pragma once

#include "Camera.h"
#include "Rhombus/Core/DeltaTime.h"
#include "Rhombus/Events/Event.h"
#include "Rhombus/Events/MouseEvent.h"
#include "Rhombus/Math/Matrix.h"
#include "Rhombus/Math/Quat.h"

namespace rhombus {

	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		void OnUpdate(DeltaTime dt);
		void OnEvent(Event& e);

		inline float GetDistance() const { return m_distance; }
		inline void SetDistance(float distance) { m_distance = distance; }

		inline void SetViewportSize(float width, float height) { m_viewportWidth = width; m_viewportHeight = height; UpdateProjection(); }

		const Mat4& GetViewMatrix() const { return m_viewMatrix; }
		Mat4 GetViewProjection() const { return m_projection * m_viewMatrix; }

		Vec3 GetUpDirection() const;
		Vec3 GetRightDirection() const;
		Vec3 GetForwardDirection() const;
		const Vec3& GetPosition() const { return m_position; }
		Quat GetOrientation() const;

		float GetPitch() const { return m_pitch; }
		float GetYaw() const { return m_yaw; }
	private:
		void UpdateProjection();
		void UpdateView();

		bool OnMouseScroll(MouseScrolledEvent& e);

		void MousePan(const Vec2& delta);
		void MouseRotate(const Vec2& delta);
		void MouseZoom(float delta);

		Vec3 CalculatePosition() const;

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;
	private:
		float m_fov = 45.0f, m_aspectRatio = 1.778f, m_nearClip = 0.1f, m_farClip = 1000.0f;

		Mat4 m_viewMatrix;
		Vec3 m_position = { 0.0f, 0.0f, 0.0f };
		Vec3 m_focalPoint = { 0.0f, 0.0f, 0.0f };

		Vec2 m_initialMousePosition = { 0.0f, 0.0f };

		float m_distance = 960.0f;
		float m_pitch = 0.0f, m_yaw = 0.0f;

		float m_viewportWidth = 1280, m_viewportHeight = 720;
	};

}
