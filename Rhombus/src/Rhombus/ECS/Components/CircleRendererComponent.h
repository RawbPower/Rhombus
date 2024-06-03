#pragma 

#include "Component.h"

#include <glm/glm.hpp>

namespace rhombus
{
	class CircleRendererComponent : public ComponentBase
	{
	public:
		CircleRendererComponent() = default;
		CircleRendererComponent(const CircleRendererComponent&) = default;

		glm::vec4 m_color{ 1.0f, 1.0f, 1.0f, 1.0f };
		float m_thickness = 1.0f;
		float m_fade = 0.0f;
	};
}
