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

	private:
		std::vector<Tileset> m_tilesets;
	};
}
