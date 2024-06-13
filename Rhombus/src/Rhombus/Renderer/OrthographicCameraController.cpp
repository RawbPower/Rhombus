#include "rbpch.h"
#include "OrthographicCameraController.h"

#include "Rhombus/Core/Input.h"
#include "Rhombus/Core/KeyCodes.h"

#include "Rhombus/Math/Math.h"

namespace rhombus {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Bounds({ -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel }), m_Camera(m_Bounds.left, m_Bounds.right, m_Bounds.bottom, m_Bounds.top), m_Rotation(rotation)
	{

	}

	void OrthographicCameraController::OnUpdate(DeltaTime dt)
	{
		RB_PROFILE_FUNCTION();

		// Camera Movement
		if (Input::IsKeyPressed(RB_KEY_A) || Input::IsKeyPressed(RB_KEY_LEFT))
		{
			m_CameraPosition.x -= cos(m_CameraRotation * math::DegToRad) * m_CameraTranslationSpeed * dt;
			m_CameraPosition.y -= sin(m_CameraRotation * math::DegToRad) * m_CameraTranslationSpeed * dt;
		}
		else if (Input::IsKeyPressed(RB_KEY_D) || Input::IsKeyPressed(RB_KEY_RIGHT))
		{
			m_CameraPosition.x += cos(m_CameraRotation * math::DegToRad) * m_CameraTranslationSpeed * dt;
			m_CameraPosition.y += sin(m_CameraRotation * math::DegToRad) * m_CameraTranslationSpeed * dt;
		}

		if (Input::IsKeyPressed(RB_KEY_S) || Input::IsKeyPressed(RB_KEY_DOWN))
		{
			m_CameraPosition.x -= -sin(m_CameraRotation * math::DegToRad) * m_CameraTranslationSpeed * dt;
			m_CameraPosition.y -= cos(m_CameraRotation * math::DegToRad) * m_CameraTranslationSpeed * dt;
		}
		else if (Input::IsKeyPressed(RB_KEY_W) || Input::IsKeyPressed(RB_KEY_UP))
		{
			m_CameraPosition.x += -sin(m_CameraRotation * math::DegToRad) * m_CameraTranslationSpeed * dt;
			m_CameraPosition.y += cos(m_CameraRotation * math::DegToRad) * m_CameraTranslationSpeed * dt;
		}

		// Camera Rotation
		if (m_Rotation)
		{
			// For some reason the positive rotation is clockwise
			if (Input::IsKeyPressed(RB_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * dt;

			if (Input::IsKeyPressed(RB_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * dt;

			// Keep rotation between -180 to 180 degrees
			if (m_CameraRotation > 180.0f)
				m_CameraRotation -= 360.0f;
			else if (m_CameraRotation <= -180.0f)
				m_CameraRotation += 360.0f;

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		RB_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(RB_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(RB_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	void OrthographicCameraController::CalculateView()
	{
		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjection(m_Bounds.left, m_Bounds.right, m_Bounds.bottom, m_Bounds.top);
	}

	void OrthographicCameraController::OnResize(float width, float height)
	{
		m_AspectRatio = width / height;
		CalculateView();
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		RB_PROFILE_FUNCTION();

		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		CalculateView();
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		RB_PROFILE_FUNCTION();

		OnResize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}
}