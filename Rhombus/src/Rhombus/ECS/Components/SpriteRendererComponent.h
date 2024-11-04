#pragma once

#include "Component.h"

#include "Rhombus/Renderer/Texture.h"
#include "Rhombus/Core/Color.h"

namespace rhombus
{
	class SpriteRendererComponent : public ComponentBase
	{
	public:
		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;

		Color& GetColor() { return m_color; }
		Color GetColor() const { return m_color; }
		void SetColor(Color color) { m_color = color; }

		Vec2 GetSpriteSize() const { return m_texture ? Vec2(m_texture->GetWidth(), m_texture->GetHeight()) : Vec2(0.0f); }

		Ref<Texture2D> m_texture;

	private:
		Color m_color{ 1.0f, 1.0f, 1.0f, 1.0f };
	};
}
