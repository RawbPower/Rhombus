#include "rbpch.h"
#include "Tileset.h"

#include "Rhombus/Project/Project.h"

#define YAML_CPP_STATIC_DEFINE		// Needed for yaml static library to work for some reason
#include <yaml-cpp/yaml.h>

namespace rhombus 
{
	Tileset::Tileset(Ref<Texture2D> tileset, uint32_t rows, uint32_t cols)
		: m_Tileset(tileset), m_iRowCount(rows), m_iColumnCount(cols)
	{
		SubTexture2D::SliceTexture(m_Tileset, m_iRowCount, m_iColumnCount, 1, m_Tiles);
	}

	Ref<Tileset> Tileset::Load(const std::string& path)
	{
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(path);
		}
		catch (YAML::ParserException e)
		{
			RB_CORE_ERROR("Failed to load tileset file '{0}'\n     {1}", path, e.what());
			return nullptr;
		}

		auto tilesetNode = data["Tileset"];
		if (!tilesetNode)
			return nullptr;

		Ref<Texture2D> tileSheet = nullptr;
		uint32_t rows, columns;

		if (tilesetNode["TileSheet"].Type() != YAML::NodeType::Undefined)
		{
			std::string texturePathString = tilesetNode["TileSheet"].as<std::string>();
			auto texturePath = Project::GetAssetFileSystemPath(texturePathString);
			tileSheet = Texture2D::Create(texturePath.string());
		}

		rows = tilesetNode["Rows"].as<uint32_t>();
		columns = tilesetNode["Columns"].as<uint32_t>();

		return CreateRef<Tileset>(tileSheet, rows, columns);
	}
}