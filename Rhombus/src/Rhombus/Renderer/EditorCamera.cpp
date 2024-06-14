#include "rbpch.h"
#include "EditorCamera.h"

#include "Rhombus/Core/Input.h"
#include "Rhombus/Core/KeyCodes.h"
#include "Rhombus/Core/MouseButtonCodes.h"

#include "Rhombus/Math/Math.h"

#include <SDL.h>

namespace rhombus {

	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: m_fov(fov), m_aspectRatio(aspectRatio), m_nearClip(nearClip), m_farClip(farClip), Camera(Mat4::Perspective(fov * math::DegToRad, aspectRatio, nearClip, farClip))
	{
		UpdateView();
	}

	void EditorCamera::UpdateProjection()
	{
		m_aspectRatio = m_viewportWidth / m_viewportHeight;
		m_projection = Mat4::Perspective(m_fov * math::DegToRad, m_aspectRatio, m_nearClip, m_farClip);
	}

	void EditorCamera::UpdateView()
	{
		//m_yaw = m_pitch = 0.0f; // Lock the camera's rotation
		m_position = CalculatePosition();

		Quat orientation = GetOrientation();
		m_viewMatrix = math::Translate(Mat4::Identity(), m_position) * orientation.ToMat4();
		m_viewMatrix = m_viewMatrix.Inverse();
	}

	std::pair<float, float> EditorCamera::PanSpeed() const
	{
		float x = std::min(m_viewportWidth / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(m_viewportHeight / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCamera::RotationSpeed() const
	{
		return 0.8f;
	}

	float EditorCamera::ZoomSpeed() const
	{
		float distance = m_distance * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f); // max speed = 100
		return speed;
	}

	void EditorCamera::OnUpdate(DeltaTime dt)
	{
		if (Input::IsKeyPressed(RB_KEY_LEFT_ALT))
		{
			const Vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
			Vec2 delta = (mouse - m_initialMousePosition) * 0.003f;
			m_initialMousePosition = mouse;

			if (Input::IsMouseButtonPressed(RB_MOUSE_BUTTON_MIDDLE))
				MousePan(delta);
			else if (Input::IsMouseButtonPressed(RB_MOUSE_BUTTON_LEFT))
				MouseRotate(delta);
			else if (Input::IsMouseButtonPressed(RB_MOUSE_BUTTON_RIGHT))
				MouseZoom(delta.y);
		}
		else
		{
			const Vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
			Vec2 delta = (mouse - m_initialMousePosition) * 0.003f;
			m_initialMousePosition = mouse;
			if (Input::IsMouseButtonPressed(RB_MOUSE_BUTTON_MIDDLE))
			{
				MouseRotate(delta);
			}
		}
		
		if (Input::IsKeyPressed(RB_KEY_L))
		{
			// Reset rotation
			m_yaw = m_pitch = 0.0f;
		}

		UpdateView();
	}

	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(RB_BIND_EVENT_FN(EditorCamera::OnMouseScroll));
	}

	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		float delta = e.GetYOffset() * 0.1f;
		MouseZoom(delta);
		UpdateView();
		return false;
	}

	void EditorCamera::MousePan(const Vec2& delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		m_focalPoint += -GetRightDirection() * delta.x * xSpeed * m_distance;
		m_focalPoint += GetUpDirection() * delta.y * ySpeed * m_distance;
	}

	void EditorCamera::MouseRotate(const Vec2& delta)
	{
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		m_yaw += yawSign * delta.x * RotationSpeed();
		m_pitch += delta.y * RotationSpeed();
	}

	void EditorCamera::MouseZoom(float delta)
	{
		m_distance -= delta * ZoomSpeed();
		if (m_distance < 1.0f)
		{
			m_focalPoint += GetForwardDirection();
			m_distance = 1.0f;
		}
	}

	Vec3 EditorCamera::GetUpDirection() const
	{
		return math::Rotate(GetOrientation(), Vec3(0.0f, 1.0f, 0.0f));
	}

	Vec3 EditorCamera::GetRightDirection() const
	{
		return math::Rotate(GetOrientation(), Vec3(1.0f, 0.0f, 0.0f));
	}

	Vec3 EditorCamera::GetForwardDirection() const
	{
		return math::Rotate(GetOrientation(), Vec3(0.0f, 0.0f, -1.0f));
	}

	Vec3 EditorCamera::CalculatePosition() const
	{
		return m_focalPoint - GetForwardDirection() * m_distance;
	}

	Quat EditorCamera::GetOrientation() const
	{
		return Quat(Vec3(-m_pitch, -m_yaw, 0.0f));
	}

}