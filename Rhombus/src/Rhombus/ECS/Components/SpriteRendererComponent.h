#pragma once

#include "Component.h"

#include "Rhombus/Renderer/Texture.h"

#include <glm/glm.hpp>

namespace rhombus
{
	class SpriteRendererComponent : public ComponentBase
	{
	public:
		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;

		glm::vec4& GetColor() { return m_color; }
		glm::vec4 GetColor() const { return m_color; }
		void SetColor(glm::vec4 color) { m_color = color; }

		Ref<Texture2D> m_texture;

	private:
		glm::vec4 m_color{ 1.0f, 1.0f, 1.0f, 1.0f };
	};
}
