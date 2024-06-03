#pragma once

#include "Component.h"

#include "Rhombus/ECS/SceneCamera.h"

namespace rhombus
{
	class CameraComponent : public ComponentBase
	{
	public:
		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

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
