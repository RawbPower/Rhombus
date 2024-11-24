#include "rbpch.h"
#include "SpriteRendererComponent.h"

namespace rhombus
{
	Vec2 SpriteRendererComponent::GetSpriteSize() const 
	{
		if (UseSubTexture())
		{
			return m_subtexture ? Vec2(m_subtexture->GetWidth(), m_subtexture->GetHeight()) : Vec2(0.0f);
		}
		else
		{
			return m_texture ? Vec2(m_texture->GetWidth(), m_texture->GetHeight()) : Vec2(0.0f);
		}
	}

	void SpriteRendererComponent::UpdateSubTexture() 
	{
		if (m_texture)
		{
			Vec2 spriteSize = Vec2(((float)m_texture->GetWidth() / (float)m_columns) - 2.0f * m_padding, ((float)m_texture->GetHeight() / (float)m_rows) - 2.0f * m_padding);
			if (m_subtexture)
			{
				m_subtexture->UpdateTexCoords(Vec2(m_frame % m_columns, m_rows - 1.0f - m_frame / m_columns), spriteSize, m_padding);
			}
			else
			{
				m_subtexture = SubTexture2D::CreateFromCoords(m_texture, Vec2(m_frame % m_columns, m_rows - 1.0f - m_frame / m_rows), spriteSize, m_padding);
			}
		}
	}
}