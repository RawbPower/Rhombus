#pragma once

#include "Rhombus/Tiles/Tileset.h"

#include <unordered_map>

namespace rhombus
{
	typedef std::vector<std::vector<int>> TileGrid;

	const uint32_t DEFAULT_GRID_DIMENSIONS = 32;
	const uint32_t DEFAULT_TILE_SIZE = 16;

	class TileMap
	{
	public:
		TileMap();

		Ref<SubTexture2D> GetTile(uint32_t i, uint32_t j) const { return m_tileGrid[i][j] < 0 ? nullptr : m_tilesets[0]->GetTile(m_tileGrid[i][j]); }
		void SetTile(std::string tilesetID, int tileIndex, uint32_t i, uint32_t j) { m_tileGrid[i][j] = tileIndex; }
		void ClearTile(uint32_t i, uint32_t j) { m_tileGrid[i][j] = -1; }

		bool ContainsTileset(std::string id) const;
		const Ref<Tileset> GetTileset(std::string id) const;
		const Ref<Tileset> GetTileset(int i) const;
		std::vector<Ref<Tileset>> GetTilesets() const { return m_tilesets; };
		Ref<Tileset> CreateTileset(Ref<Tileset>& tileset);

		Vec2 GetTileSize() const { return m_tileSize; }
		uint32_t GetGridWidth() const { return m_gridWidth; }
		uint32_t GetGridHeight() const { return m_gridHeight; }

		static Ref<TileMap> Create();

	public:
		TileGrid m_tileGrid;

	private:
		//std::unordered_map<std::string, Ref<Tileset>> m_tilesets;
		std::unordered_map<std::string, uint32_t> m_idToIndexMap;
		std::vector<Ref<Tileset>> m_tilesets;
		uint32_t m_gridWidth;
		uint32_t m_gridHeight;
		Vec2 m_tileSize;
	};
}

