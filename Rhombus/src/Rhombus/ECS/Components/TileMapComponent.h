#pragma once

#include "Component.h"
#include "Rhombus/Tiles/TileMap.h"

namespace rhombus
{
	class TileMapComponent : public ComponentBase
	{
	public:
		TileMapComponent() = default;
		TileMapComponent(const TileMapComponent& other) = default;

		Ref<TileMap> GetTileMap() { return m_tilemap; }
		void CreateTileMap() { m_tilemap = CreateRef<TileMap>(); };

		Ref<TileMap> m_tilemap;

	private:
		//Ref<TileMap> m_tilemap;
	};
}
