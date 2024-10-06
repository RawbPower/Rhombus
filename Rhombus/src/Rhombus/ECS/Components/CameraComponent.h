#pragma once

#include "Component.h"

#include "Rhombus/Scenes/SceneCamera.h"
#include "Rhombus/Core/Application.h"

namespace rhombus
{
	class CameraComponent : public ComponentBase
	{
	public:
		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

		virtual void OnComponentAdded() override
		{
			Viewport viewport = Application::Get().GetViewport();
			if (viewport.width > 0 && viewport.height > 0)
			{
				GetCamera().SetViewportResize(viewport.width, viewport.height);
			}
		}

		SceneCamera& GetCamera() { return m_camera; }
		void SetIsPrimaryCamera(bool primary) { m_primary = primary; }
		bool& GetIsPrimaryCamera() { return m_primary; }

		void SetHasFixedAspectRatio(bool fixedAspectRatio) { m_fixedAspectRatio = fixedAspectRatio; }
		bool& GetHasFixedAspectRatio() { return m_fixedAspectRatio; }
	private:
		SceneCamera m_camera;
		bool m_primary = true;	// Maybe move this to the scene and out of the component
		bool m_fixedAspectRatio = false;
	};
}
