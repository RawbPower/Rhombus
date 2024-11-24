#include "rbpch.h"
#include "TileMap.h"

namespace rhombus
{
	TileMap::TileMap() : m_gridWidth(DEFAULT_GRID_DIMENSIONS), m_gridHeight(DEFAULT_GRID_DIMENSIONS), m_tileSize(DEFAULT_TILE_SIZE)
	{
	}

	bool TileMap::ContainsTileset(std::string id) const
	{
		return m_idToIndexMap.find(id) != m_idToIndexMap.end();
	}

	const Ref<Tileset> TileMap::GetTileset(std::string id) const
	{
		return m_tilesets[m_idToIndexMap.at(id)];
	}

	const Ref<Tileset> TileMap::GetTileset(int i) const
	{
		return m_tilesets[i];
	}

	Ref<Tileset> TileMap::CreateTileset(Ref<Tileset>& tileset)
	{
		m_tilesets.push_back(CreateRef<Tileset>(tileset->GetID(), tileset->GetPath(), tileset->GetTileset(), tileset->GetRowCount(), tileset->GetColumnCount(), tileset->GetPadding()));
		m_idToIndexMap[tileset->GetID()] = m_tilesets.size() - 1;;
		return m_tilesets.back();
	}

	Ref<TileMap> TileMap::Create()
	{
		Ref<TileMap> tilemap = CreateRef<TileMap>();
		tilemap->m_tileGrid.resize(DEFAULT_GRID_DIMENSIONS, std::vector<int>(DEFAULT_GRID_DIMENSIONS, -1));
		return tilemap;
	}
}