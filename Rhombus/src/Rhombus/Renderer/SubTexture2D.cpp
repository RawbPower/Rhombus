#include "rbpch.h"
#include "SubTexture2D.h"

namespace rhombus
{
	// Margin within the texture coords to avoid sampling from the edge between two sprites (texture bleeding)
	// May need to look into a less arbitrary solution to this
	static float s_textureBleedMargin = 0.01f;

	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const Vec2& min, const Vec2& max)
		: m_Texture(texture)
	{
		m_TexCoords[0] = { min.x, min.y };
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = { max.x, max.y };
		m_TexCoords[3] = { min.x, max.y };
	}

	Ref<SubTexture2D> SubTexture2D::CreateFromCoords(const Ref<Texture2D>& texture, const Vec2& coords, const Vec2& cellSize, const Vec2& spriteSize)
	{
		Vec2 min = { (coords.x * cellSize.x + s_textureBleedMargin) / texture->GetWidth(), (coords.y * cellSize.y + s_textureBleedMargin) / texture->GetHeight() };
		Vec2 max = { ((coords.x + spriteSize.x) * cellSize.x - s_textureBleedMargin) / texture->GetWidth(), ((coords.y + spriteSize.y) * cellSize.y - s_textureBleedMargin) / texture->GetHeight() };

		return std::make_shared<SubTexture2D>(texture, min, max);
	}
}