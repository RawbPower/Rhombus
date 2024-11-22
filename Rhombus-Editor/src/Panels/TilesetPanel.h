#pragma once

#include "Rhombus/Renderer/Texture.h"
#include "Rhombus/Renderer/SubTexture2D.h"
#include "Rhombus/Renderer/Tileset.h"

namespace rhombus
{
	class TilesetPanel
	{
	public:

		TilesetPanel();

		void OnImGuiRender();
		void SetTileset(const std::string& path);

		const Ref<SubTexture2D> GetSelectedTile() const { return m_iSelectedTileIndex >= 0 ? m_Tileset->GetTile(m_iSelectedTileIndex) : nullptr; }
		int GetSelectedTileIndex() const { return m_iSelectedTileIndex; }

		std::string GetTilesetID() const { return m_Tileset->GetID(); }
		Ref<Tileset> GetTileset() const { return m_Tileset; }

	private:
		Ref<Tileset> m_Tileset;

		int m_iSelectedTileIndex;
	};
}