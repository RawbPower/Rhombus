#pragma once

#include "Component.h"

#include <glm/glm.hpp>

namespace rhombus
{
	// TODO: Add a Area2DComponent base class

	class BoxArea2DComponent : public ComponentBase
	{
	public:
		glm::vec2 m_offset = { 0.0f, 0.0f };
		glm::vec2 m_size = { 0.5f, 0.5f };

		bool m_isMouseInArea = false;

		BoxArea2DComponent() = default;
		BoxArea2DComponent(const BoxArea2DComponent& other) = default;
	};
}
