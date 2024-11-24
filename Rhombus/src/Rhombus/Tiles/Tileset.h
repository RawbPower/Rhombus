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
		Tileset(std::string name, std::string path, Ref<Texture2D> tileset, uint32_t rows, uint32_t cols, uint32_t padding);

		virtual ~Tileset() = default;

		const Ref<Texture2D> GetTileset() const { return m_Tileset; }
		Ref<SubTexture2D> GetTile(uint32_t index) const { return m_Tiles[index]; }
		const std::string GetID() const { return m_name; }
		const std::string GetPath() const { return m_path; }
		const uint32_t GetRowCount() const { return m_iRowCount; }
		const uint32_t GetColumnCount() const { return m_iColumnCount; }
		const uint32_t GetPadding() const { return m_iPadding; }
		Vec2 GetTileSize() const { return m_tileSize; }

	private:
		Ref<Texture2D> m_Tileset;

		// Tiles are stored in a sequence of rows based of the tileset texture
		std::vector<Ref<SubTexture2D>> m_Tiles;

		std::string m_name;
		std::string m_path;
		uint32_t m_iRowCount;
		uint32_t m_iColumnCount;
		uint32_t m_iPadding;
		Vec2 m_tileSize;
	};
}
