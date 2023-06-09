#pragma once

#include "Rhombus/Renderer/OrthographicCamera.h"
#include "Rhombus/Core/DeltaTime.h"

#include "Rhombus/Events/ApplicationEvent.h"
#include "Rhombus/Events/MouseEvent.h"

namespace rhombus {

	class OrthographicCameraController 
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);		// vertically 2 units

		void OnUpdate(DeltaTime dt);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		bool m_Rotation;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;	// In degrees, in the anti-clockwise direction
		float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 180.0f;
	};

}
