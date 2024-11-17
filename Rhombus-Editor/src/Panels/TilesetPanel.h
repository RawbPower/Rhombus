#pragma once

#include "Rhombus/Renderer/Texture.h"
#include "Rhombus/Renderer/SubTexture2D.h"

namespace rhombus
{
	class TilesetPanel
	{
	public:

		TilesetPanel();

		void OnImGuiRender();

		const Ref<SubTexture2D> GetSelectedTile() const { return m_iSelectedTileIndex >= 0 ? m_Tiles[m_iSelectedTileIndex] : nullptr; }

	private:
		Ref<Texture2D> m_Tileset;
		std::vector<Ref<SubTexture2D>> m_Tiles;

		uint32_t m_iRowCount;
		uint32_t m_iColumnCount;
		int m_iSelectedTileIndex;
	};
}