#include "rbpch.h"
#include "Tileset.h"

namespace rhombus 
{
	Tileset::Tileset(Ref<Texture2D> tileset, uint32_t rows, uint32_t cols)
		: m_Tileset(tileset), m_iRowCount(rows), m_iColumnCount(cols)
	{
		SubTexture2D::SliceTexture(m_Tileset, m_iRowCount, m_iColumnCount, 1, m_Tiles);
	}
}