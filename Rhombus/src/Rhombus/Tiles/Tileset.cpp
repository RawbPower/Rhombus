#include "rbpch.h"
#include "Tileset.h"

#include "Rhombus/Project/Project.h"

#define YAML_CPP_STATIC_DEFINE		// Needed for yaml static library to work for some reason
#include <yaml-cpp/yaml.h>

namespace rhombus 
{
	Tileset::Tileset(std::string name, Ref<Texture2D> tileset, uint32_t rows, uint32_t cols)
		: m_name(name), m_Tileset(tileset), m_iRowCount(rows), m_iColumnCount(cols)
	{
		SubTexture2D::SliceTexture(m_Tileset, m_iRowCount, m_iColumnCount, 1, m_Tiles);
	}
}