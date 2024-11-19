#pragma once

#include "Rhombus/Core/Core.h"

#include "Texture.h"
#include "SubTexture2D.h"

#include <vector>

class Texture2D;
class SubTexture2D;

namespace rhombus
{
	class Tileset
	{
	public:
		Tileset() = default;
		Tileset(Ref<Texture2D> tileset, uint32_t rows, uint32_t cols);

		virtual ~Tileset() = default;

		const Ref<SubTexture2D> GetTile(uint32_t index) const { return m_Tiles[index]; }
		const uint32_t GetRowCount() const { return m_iRowCount; }
		const uint32_t GetColumnCount() const { return m_iColumnCount; }

	private:
		Ref<Texture2D> m_Tileset;

		// Tiles are stored in a sequence of rows based of the tileset texture
		std::vector<Ref<SubTexture2D>> m_Tiles;

		uint32_t m_iRowCount;
		uint32_t m_iColumnCount;
	};
}
