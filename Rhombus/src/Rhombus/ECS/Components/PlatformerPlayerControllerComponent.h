#pragma once

#include "Component.h"

namespace rhombus
{
	class PlatformerPlayerControllerComponent : public ComponentBase
	{
	public:
		PlatformerPlayerControllerComponent() = default;
		PlatformerPlayerControllerComponent(const PlatformerPlayerControllerComponent& other) = default;

		float m_speed = 0.0f;
		float m_jumpHeight = 0.0f;
	private:
	};
}
