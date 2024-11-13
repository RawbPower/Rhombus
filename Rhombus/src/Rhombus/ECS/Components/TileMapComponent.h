#pragma once

#include "Component.h"

namespace rhombus
{
	// TODO: Add a Area2DComponent base class

	class TileMapComponent : public ComponentBase
	{
	public:
		TileMapComponent() = default;
		TileMapComponent(const TileMapComponent& other) = default;

	private:
		uint32_t m_tilemap[32][32];
	};
}
