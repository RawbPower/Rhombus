#include "rbpch.h"
#include "Tileset.h"

#include "Rhombus/Project/Project.h"

#define YAML_CPP_STATIC_DEFINE		// Needed for yaml static library to work for some reason
#include <yaml-cpp/yaml.h>

namespace rhombus 
{
	Tileset::Tileset(std::string name, std::string path, Ref<Texture2D> tileset, uint32_t rows, uint32_t cols, uint32_t padding)
		: m_name(name), m_Tileset(tileset), m_iRowCount(rows), m_iColumnCount(cols), m_iPadding(padding), m_path(path), m_tileSize(0.0f)
	{
		m_tileSize = Vec2(((float)m_Tileset->GetWidth() / (float)cols) - 2.0f * padding, ((float)m_Tileset->GetHeight() / (float)rows) - 2.0f * padding);
		SubTexture2D::SliceTexture(m_Tileset, m_tileSize, m_iRowCount, m_iColumnCount, m_iPadding, m_Tiles);
	}
}