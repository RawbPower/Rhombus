#include "rbpch.h"
#include "TileMapComponent.h"

namespace rhombus
{
	bool TileMapComponent::ContainsTileset(std::string id) const
	{
		return m_tilesets.find(id) != m_tilesets.end();
	}

	const Tileset& TileMapComponent::GetTileset(std::string id) const
	{
		return m_tilesets.at(id);
	}

	Tileset* TileMapComponent::CreateTileset(Ref<Tileset>& tileset)
	{
		m_tilesets[tileset->GetID()] = Tileset(tileset->GetID(), tileset->GetTileset(), tileset->GetRowCount(), tileset->GetColumnCount());
		return &m_tilesets.at(tileset->GetID());
	}
}