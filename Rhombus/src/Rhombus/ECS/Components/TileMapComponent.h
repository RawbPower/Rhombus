#pragma once

#include "Component.h"

namespace rhombus
{
	class SubTexture2D;

	class TileMapComponent : public ComponentBase
	{
	public:
		TileMapComponent() = default;
		TileMapComponent(const TileMapComponent& other) = default;

		Ref<SubTexture2D> m_tilemap[32][32] = { nullptr };

	private:
	};
}
