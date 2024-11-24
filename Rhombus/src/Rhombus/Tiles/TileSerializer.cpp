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
	YAML::Emitter& operator<<(YAML::Emitter& out, const std::vector<std::vector<int>>& t)
	{
		out << YAML::BeginSeq;
		for (int i = 0; i < t.size(); i++)
		{
			out << YAML::Flow;
			out << YAML::BeginSeq;
			for (int j = 0; j < t[i].size(); j++)
			{
				out << t[i][j];
			}
			out << YAML::EndSeq;
		}
		out << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const std::vector<Ref<Tileset>>& t)
	{
		out << YAML::BeginSeq;
		for (int i = 0; i < t.size(); i++)
		{
			out << t[i]->GetPath();
		}
		out << YAML::EndSeq;	
		return out;
	}

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
		uint32_t rows, columns, padding;

		if (tilesetNode["TileSheet"].Type() != YAML::NodeType::Undefined)
		{
			std::string texturePathString = tilesetNode["TileSheet"].as<std::string>();
			auto texturePath = Project::GetAssetFileSystemPath(texturePathString);
			tileSheet = Texture2D::Create(texturePath.string());
		}

		std::string path = filepath;
		std::string name = tilesetNode["Name"].as<std::string>();
		rows = tilesetNode["Rows"].as<uint32_t>();
		columns = tilesetNode["Columns"].as<uint32_t>();
		padding = tilesetNode["Padding"].as<uint32_t>();

		return CreateRef<Tileset>(name, path, tileSheet, rows, columns, padding);
	}

	bool TileSerializer::SerializeTileMap(const std::filesystem::path& filepath, Ref<TileMap>& tilemap)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Tilesets" << YAML::Value << tilemap->GetTilesets();
		out << YAML::Key << "TileGrid" << YAML::Value << tilemap->m_tileGrid;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
		return true;
	}

	Ref<TileMap> TileSerializer::DeserializeTileMap(const std::string& filepath)
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

		auto tilesetsNode = data["Tilesets"];
		if (!tilesetsNode)
			return nullptr;

		auto tileGridNode = data["TileGrid"];
		if (!tileGridNode)
			return nullptr;

		Ref<TileMap> tilemap = TileMap::Create();
		std::vector<std::string> tilesets = tilesetsNode.as<std::vector<std::string>>();
		for (std::string tileset : tilesets)
		{
			tilemap->CreateTileset(TileSerializer::DeserializeTileset(tileset));
		}

		std::vector<std::vector<int>> tilegrid = tileGridNode.as<std::vector<std::vector<int>>>();
		tilemap->m_tileGrid = tilegrid;

		return tilemap;
	}
}