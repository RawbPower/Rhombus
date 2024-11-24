#pragma once

#include "Component.h"

#include "Rhombus/Renderer/Texture.h"
#include "Rhombus/Renderer/SubTexture2D.h"
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

		uint32_t GetRows() const { return m_rows; }
		void SetRows(uint32_t rows) { m_rows = rows; UpdateSubTexture(); }
		uint32_t GetColumns() const { return m_columns; }
		void SetColumns(uint32_t columns) { m_columns = columns; UpdateSubTexture(); }
		uint32_t GetPadding() const { return m_padding; }
		void SetPadding(uint32_t padding) { m_padding = padding; UpdateSubTexture(); }
		uint32_t GetFrame() const { return m_frame; }
		void SetFrame(uint32_t frame) { m_frame = frame; UpdateSubTexture(); }

		bool UseSubTexture() const { return m_subtexture && (m_rows > 1 || m_columns > 1); }
		Vec2 GetSpriteSize() const;
		void UpdateSubTexture();

		Ref<Texture2D> m_texture;
		Ref<SubTexture2D> m_subtexture;

	private:
		Color m_color{ 1.0f, 1.0f, 1.0f, 1.0f };
		uint32_t m_rows = 1;
		uint32_t m_columns = 1;
		uint32_t m_padding = 0;
		uint32_t m_frame = 0;
	};
}
