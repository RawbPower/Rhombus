#pragma once

#include "Component.h"
#include "Rhombus/Renderer/Tileset.h"

namespace rhombus
{
	class TileMapComponent : public ComponentBase
	{
	public:
		TileMapComponent() = default;
		TileMapComponent(const TileMapComponent& other) = default;

		Ref<SubTexture2D> m_tilemap[32][32] = { nullptr };

		bool ContainsTileset(std::string id) const;
		const Tileset& GetTileset(std::string id) const;
		Tileset* CreateTileset(Ref<Tileset>& tileset);

	private:
		std::unordered_map<std::string, Tileset> m_tilesets;
	};
}
