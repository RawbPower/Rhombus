#pragma once

#include "Component.h"
#include "Rhombus/Core/Color.h"

namespace rhombus
{
	// TODO: Add a Area2DComponent base class

	class BoxArea2DComponent : public ComponentBase
	{
	public:
		Vec2 m_offset = { 0.0f, 0.0f };
		Vec2 m_size = { 0.5f, 0.5f };

		Color& GetDebugColor() { return m_debugColor; }

		bool m_isMouseInArea = false;

		BoxArea2DComponent() = default;
		BoxArea2DComponent(const BoxArea2DComponent& other) = default;

	private:
		Color m_debugColor{ 0.0f, 0.0f, 0.0f, 0.0f };
	};
}
