#pragma once

#include "Rhombus/Core/Core.h"

#include "Rhombus/Renderer/Texture.h"
#include "Rhombus/Renderer/SubTexture2D.h"

#include <vector>

namespace rhombus
{
	class Tileset
	{
	public:
		Tileset() = default;
		Tileset(std::string name, Ref<Texture2D> tileset, uint32_t rows, uint32_t cols);

		virtual ~Tileset() = default;

		const Ref<Texture2D> GetTileset() const { return m_Tileset; }
		Ref<SubTexture2D> GetTile(uint32_t index) const { return m_Tiles[index]; }
		const std::string GetID() const { return m_name; }
		const uint32_t GetRowCount() const { return m_iRowCount; }
		const uint32_t GetColumnCount() const { return m_iColumnCount; }

	private:
		Ref<Texture2D> m_Tileset;

		// Tiles are stored in a sequence of rows based of the tileset texture
		std::vector<Ref<SubTexture2D>> m_Tiles;

		std::string m_name;
		uint32_t m_iRowCount;
		uint32_t m_iColumnCount;
	};
}
