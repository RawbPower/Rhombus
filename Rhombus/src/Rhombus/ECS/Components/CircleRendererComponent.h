#pragma 

#include "Component.h"

#include "Rhombus/Core/Color.h"

namespace rhombus
{
	class CircleRendererComponent : public ComponentBase
	{
	public:
		CircleRendererComponent() = default;
		CircleRendererComponent(const CircleRendererComponent&) = default;

		Color m_color{ 1.0f, 1.0f, 1.0f, 1.0f };
		float m_thickness = 1.0f;
		float m_fade = 0.0f;
	};
}
