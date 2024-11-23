#include "rbpch.h"
#include "TileSerializer.h"

#include "Rhombus/Project/Project.h"
#include "Rhombus/Renderer/Texture.h"
#include "Rhombus/Renderer/SubTexture2D.h"

#include <fstream>

#define YAML_CPP_STATIC_DEFINE		// Needed for yaml static library to work for some reason
#include <yaml-cpp/yaml.h>

namespace rhombus
{
	bool TileSerializer::SerializeTileset(const std::filesystem::path& filepath)
	{
		return false;
	}

	Ref<Tileset> TileSerializer::DeserializeTileset(const std::string& filepath)
	{
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(filepath);
		}
		catch (YAML::ParserException e)
		{
			RB_CORE_ERROR("Failed to load tileset file '{0}'\n     {1}", filepath, e.what());
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

		std::string name = tilesetNode["Name"].as<std::string>();
		rows = tilesetNode["Rows"].as<uint32_t>();
		columns = tilesetNode["Columns"].as<uint32_t>();

		return CreateRef<Tileset>(name, tileSheet, rows, columns);
	}

	bool TileSerializer::SerializeTileMap(const std::filesystem::path& filepath)
	{
		return false;
	}

	bool TileSerializer::DeserializeTileMap(const std::filesystem::path& filepath)
	{
		return false;
	}
}