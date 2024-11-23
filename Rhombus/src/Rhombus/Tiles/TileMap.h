#pragma once

#include "Rhombus/Tiles/Tileset.h"

#include <unordered_map>

namespace rhombus
{
	class TileMap
	{
	public:
		TileMap();

		Ref<SubTexture2D> GetTile(uint32_t i, uint32_t j) const { return m_tileGrid[i][j]; }
		void SetTile(Ref<SubTexture2D>& tile, uint32_t i, uint32_t j) { m_tileGrid[i][j] = tile; }
		void ClearTile(uint32_t i, uint32_t j) { m_tileGrid[i][j] = nullptr; }

		bool ContainsTileset(std::string id) const;
		const Tileset& GetTileset(std::string id) const;
		Tileset* CreateTileset(Ref<Tileset>& tileset);

		static Ref<TileMap> Create();

	public:
		Ref<SubTexture2D> m_tileGrid[32][32];

	private:
		std::unordered_map<std::string, Tileset> m_tilesets;
	};
}

