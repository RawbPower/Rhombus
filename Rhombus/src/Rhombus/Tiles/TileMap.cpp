#include "rbpch.h"
#include "TileMap.h"

namespace rhombus
{
	TileMap::TileMap()
	{
		for (int i = 0; i < 32; i++)
		{
			for (int j = 0; j < 32; j++)
			{
				m_tileGrid[i][j] = nullptr;
			}
		}
	}

	bool TileMap::ContainsTileset(std::string id) const
	{
		return m_tilesets.find(id) != m_tilesets.end();
	}

	const Tileset& TileMap::GetTileset(std::string id) const
	{
		return m_tilesets.at(id);
	}

	Tileset* TileMap::CreateTileset(Ref<Tileset>& tileset)
	{
		m_tilesets[tileset->GetID()] = Tileset(tileset->GetID(), tileset->GetTileset(), tileset->GetRowCount(), tileset->GetColumnCount());
		return &m_tilesets.at(tileset->GetID());
	}

	Ref<TileMap> TileMap::Create()
	{
		return std::make_shared<TileMap>();
	}
}