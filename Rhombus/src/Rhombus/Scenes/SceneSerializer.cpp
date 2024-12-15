#include "rbpch.h"
#include "SceneSerializer.h"

#include "Entity.h"
#include "SceneGraphNode.h"

#include "Rhombus/Project/Project.h"
#include "Rhombus/Tiles/TileSerializer.h"
#include "Rhombus/Animation/AnimationSerializer.h"

#include "Rhombus/ECS/Components/Area2DComponent.h"
#include "Rhombus/ECS/Components/AnimatorComponent.h"
#include "Rhombus/ECS/Components/CameraComponent.h"
#include "Rhombus/ECS/Components/CircleRendererComponent.h"
#include "Rhombus/ECS/Components/Collider2DComponent.h"
#include "Rhombus/ECS/Components/Rigidbody2DComponent.h"
#include "Rhombus/ECS/Components/PixelPlatformerBodyComponent.h"
#include "Rhombus/ECS/Components/PlatformerPlayerControllerComponent.h"
#include "Rhombus/ECS/Components/ScriptComponent.h"
#include "Rhombus/ECS/Components/SpriteRendererComponent.h"
#include "Rhombus/ECS/Components/TransformComponent.h"
#include "Rhombus/ECS/Components/TileMapComponent.h"

namespace rhombus
{
	static std::string RigidBody2DBodyTypeToString(Rigidbody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
		case Rigidbody2DComponent::BodyType::Static:    return "Static";
		case Rigidbody2DComponent::BodyType::Dynamic:   return "Dynamic";
		case Rigidbody2DComponent::BodyType::Kinematic: return "Kinematic";
		}

		Log::Assert(false, "Unknown body type");
		return {};
	}

	static Rigidbody2DComponent::BodyType RigidBody2DBodyTypeFromString(const std::string& bodyTypeString)
	{
		if (bodyTypeString == "Static")    return Rigidbody2DComponent::BodyType::Static;
		if (bodyTypeString == "Dynamic")   return Rigidbody2DComponent::BodyType::Dynamic;
		if (bodyTypeString == "Kinematic") return Rigidbody2DComponent::BodyType::Kinematic;

		Log::Assert(false, "Unknown body type");
		return Rigidbody2DComponent::BodyType::Static;
	}

	static std::string PixelPlatformerBodyTypeToString(PixelPlatformerBodyComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
		case PixelPlatformerBodyComponent::BodyType::Static:    return "Static";
		case PixelPlatformerBodyComponent::BodyType::Dynamic:   return "Dynamic";
		case PixelPlatformerBodyComponent::BodyType::Kinematic: return "Kinematic";
		}

		Log::Assert(false, "Unknown body type");
		return {};
	}

	static PixelPlatformerBodyComponent::BodyType PixelPlatformerBodyTypeFromString(const std::string& bodyTypeString)
	{
		if (bodyTypeString == "Static")    return PixelPlatformerBodyComponent::BodyType::Static;
		if (bodyTypeString == "Dynamic")   return PixelPlatformerBodyComponent::BodyType::Dynamic;
		if (bodyTypeString == "Kinematic") return PixelPlatformerBodyComponent::BodyType::Kinematic;

		Log::Assert(false, "Unknown body type");
		return PixelPlatformerBodyComponent::BodyType::Static;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: m_scene(scene)
	{
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity, Ref<Scene>& scene)
	{
		Log::Assert(entity.HasComponent<IDComponent>(), "Entity does not have a UUID");

		out << YAML::BeginMap; // Entity
		out << YAML::Key << "Entity" << YAML::Value << entity.GetUUID();
		out << YAML::Key << "Enabled" << YAML::Value << !scene->IsEntityDisabled(entity);

		bool hasParent = !entity.GetSceneGraphNode()->GetParent()->GetIsRootNode();
		if (hasParent)
		{
			Entity parent = entity.GetSceneGraphNode()->GetParent()->GetEntity();
			out << YAML::Key << "Parent" << YAML::Value << parent.GetUUID();
		}

		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap; // TagComponent

			auto& tag = entity.GetComponent<TagComponent>().m_tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap; // TagComponent
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap; // TransformComponent

			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Position" << YAML::Value << tc.GetPosition();
			out << YAML::Key << "Rotation" << YAML::Value << tc.GetRotation();
			out << YAML::Key << "Scale" << YAML::Value << tc.GetScale();

			out << YAML::EndMap; // TransformComponent
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap; // CameraComponent

			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.GetCamera();

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap; // Camera
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveVerticalFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
			out << YAML::Key << "PixelPerfect" << YAML::Value << camera.GetPixelPerfect();
			out << YAML::EndMap; // Camera

			out << YAML::Key << "Primary" << YAML::Value << cameraComponent.GetIsPrimaryCamera();
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.GetHasFixedAspectRatio();

			out << YAML::EndMap; // CameraComponent
		}

		if (entity.HasComponent<ScriptComponent>())
		{
			auto& scriptComponent = entity.GetComponent<ScriptComponent>();

			out << YAML::Key << "ScriptComponent";
			out << YAML::BeginMap;	// ScriptComponent
			out << YAML::Key << "ScriptName" << YAML::Value << scriptComponent.m_scriptName;
			out << YAML::EndMap;	// ScriptComponent
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap; // SpriteRendererComponent

			auto& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.GetColor();
			out << YAML::Key << "Rows" << YAML::Value << spriteRendererComponent.GetRows();
			out << YAML::Key << "Columns" << YAML::Value << spriteRendererComponent.GetColumns();
			out << YAML::Key << "Padding" << YAML::Value << spriteRendererComponent.GetPadding();
			out << YAML::Key << "Frame" << YAML::Value << spriteRendererComponent.GetFrame();
			if (spriteRendererComponent.m_texture)
			{
				out << YAML::Key << "Texture" << YAML::Value << Project::GetAssetFileLocalPath(spriteRendererComponent.m_texture->GetPath());
			}

			out << YAML::EndMap; // SpriteRendererComponent
		}

		if (entity.HasComponent<CircleRendererComponent>())
		{
			out << YAML::Key << "CircleRendererComponent";
			out << YAML::BeginMap; // CircleRendererComponent

			auto& circleRendererComponent = entity.GetComponent<CircleRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << circleRendererComponent.m_color;
			out << YAML::Key << "Thickness" << YAML::Value << circleRendererComponent.m_thickness;
			out << YAML::Key << "Fade" << YAML::Value << circleRendererComponent.m_fade;

			out << YAML::EndMap; // CircleRendererComponent
		}

		if (entity.HasComponent<Rigidbody2DComponent>())
		{
			out << YAML::Key << "Rigidbody2DComponent";
			out << YAML::BeginMap; // Rigidbody2DComponent

			auto& rigidbody2DComponent = entity.GetComponent<Rigidbody2DComponent>();
			out << YAML::Key << "BodyType" << YAML::Value << RigidBody2DBodyTypeToString(rigidbody2DComponent.m_type);
			out << YAML::Key << "FixedRotation" << YAML::Value << rigidbody2DComponent.m_fixedRotation;

			out << YAML::EndMap; // Rigidbody2DComponent
		}

		if (entity.HasComponent<PixelPlatformerBodyComponent>())
		{
			out << YAML::Key << "PixelPlatformerBodyComponent";
			out << YAML::BeginMap; // PixelPlatformerBodyComponent

			auto& pixelPlatformerBodyComponent = entity.GetComponent<PixelPlatformerBodyComponent>();
			out << YAML::Key << "BodyType" << YAML::Value << PixelPlatformerBodyTypeToString(pixelPlatformerBodyComponent.m_type);
			out << YAML::Key << "FixedRotation" << YAML::Value << pixelPlatformerBodyComponent.m_fixedRotation;

			out << YAML::EndMap; // PixelPlatformerBodyComponent
		}

		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			out << YAML::Key << "BoxCollider2DComponent";
			out << YAML::BeginMap; // BoxCollider2DComponent

			auto& collider2DComponent = entity.GetComponent<BoxCollider2DComponent>();
			out << YAML::Key << "Offset" << YAML::Value << collider2DComponent.m_offset;
			out << YAML::Key << "Size" << YAML::Value << collider2DComponent.m_size;
			out << YAML::Key << "Density" << YAML::Value << collider2DComponent.m_density;
			out << YAML::Key << "Friction" << YAML::Value << collider2DComponent.m_friction;
			out << YAML::Key << "Restitution" << YAML::Value << collider2DComponent.m_restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << collider2DComponent.m_restitutionThreshold;

			out << YAML::EndMap; // BoxCollider2DComponent
		}

		if (entity.HasComponent<CircleCollider2DComponent>())
		{
			out << YAML::Key << "CircleCollider2DComponent";
			out << YAML::BeginMap; // CircleCollider2DComponent

			auto& collider2DComponent = entity.GetComponent<CircleCollider2DComponent>();
			out << YAML::Key << "Offset" << YAML::Value << collider2DComponent.m_offset;
			out << YAML::Key << "Radius" << YAML::Value << collider2DComponent.m_radius;
			out << YAML::Key << "Density" << YAML::Value << collider2DComponent.m_density;
			out << YAML::Key << "Friction" << YAML::Value << collider2DComponent.m_friction;
			out << YAML::Key << "Restitution" << YAML::Value << collider2DComponent.m_restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << collider2DComponent.m_restitutionThreshold;

			out << YAML::EndMap; // CircleCollider2DComponent
		}

		if (entity.HasComponent<BoxArea2DComponent>())
		{
			out << YAML::Key << "BoxArea2DComponent";
			out << YAML::BeginMap; // BoxArea2DComponent

			auto& area2DComponent = entity.GetComponent<BoxArea2DComponent>();
			out << YAML::Key << "Offset" << YAML::Value << area2DComponent.m_offset;
			out << YAML::Key << "Size" << YAML::Value << area2DComponent.m_size;
			out << YAML::Key << "DebugColor" << YAML::Value << area2DComponent.GetDebugColor();

			out << YAML::EndMap; // BoxArea2DComponent
		}

		if (entity.HasComponent<TileMapComponent>())
		{
			auto& tilemapComponent = entity.GetComponent<TileMapComponent>();
			std::string path = "assets\\tilemaps\\" + entity.GetName() + ".rtm";
			TileSerializer::SerializeTileMap(path, tilemapComponent.m_tilemap);

			out << YAML::Key << "TileMapComponent";
			out << YAML::BeginMap; // TileMapComponent
			out << YAML::Key << "TileMap" << YAML::Value << path;
			out << YAML::EndMap; // TileMapComponent
		}

		if (entity.HasComponent<PlatformerPlayerControllerComponent>())
		{
			out << YAML::Key << "PlatformerPlayerControllerComponent";
			out << YAML::BeginMap; // PlatformerPlayerControllerComponent

			auto& controller = entity.GetComponent<PlatformerPlayerControllerComponent>();
			out << YAML::Key << "Speed" << YAML::Value << controller.m_speed;
			out << YAML::Key << "JumpHeight" << YAML::Value << controller.m_jumpHeight;

			out << YAML::EndMap; // PlatformerPlayerControllerComponent
		}

		if (entity.HasComponent<AnimatorComponent>())
		{
			auto& animatorComponent = entity.GetComponent<AnimatorComponent>();

			out << YAML::Key << "AnimatorComponent";
			out << YAML::BeginMap;	// AnimatorComponent
			out << YAML::Key << "FilePath" << YAML::Value << animatorComponent.m_filePath;
			out << YAML::EndMap;	// AnimatorComponent
		}

		scene->SerializeEntity(&out, entity);

		out << YAML::EndMap; // Entity
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		Serialize(filepath, m_scene->GetAllEntities());
	}

	void SceneSerializer::Serialize(const std::string& filepath, const std::vector<EntityID>& orderedEntities)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		for (EntityID e : orderedEntities)
		{
			Entity entity = { e, m_scene.get() };
			if (!entity)
				return;

			SerializeEntity(out, entity, m_scene);
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
		// Not implemented
		Log::Assert(false, "");
	}

	bool SceneSerializer::Deserialize(const std::string& filepath, bool includeDisabledEntities)
	{ 
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(filepath);
		}
		catch (YAML::ParserException e)
		{
			Log::Error("Failed to load .hazel file '%s'\n     %s", filepath, e.what());
			return false;
		}

		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		Log::Debug("Deserializing scene '%s'", sceneName.c_str());

		std::unordered_map<EntityID, bool>& entityEnabledMap = m_scene->GetEntityEnabledMap();

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>();
				bool enabled = entity["Enabled"].as<bool>();

				if (!enabled && !includeDisabledEntities)
				{
					continue;
				}

				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				//RB_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);
				Log::Debug("Deserialized entity with ID = %u, name = %s", uuid, name.c_str());

				Entity deserializedEntity = m_scene->CreateEntityWithUUID(uuid, name);
				entityEnabledMap[(EntityID)deserializedEntity] = enabled;

				if (entity["Parent"])
				{
					uint64_t parentUUID = entity["Parent"].as<uint64_t>();
					Entity parentEntity = m_scene->GetEntityByUUID(parentUUID);
					std::string parentName = parentEntity.GetName();
					Ref<SceneGraphNode> parentSceneGraphNode = parentEntity.GetSceneGraphNode();
					parentSceneGraphNode->AddChild(Ref<SceneGraphNode>(deserializedEntity.GetSceneGraphNode()));
				}

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					// Entities always have transforms
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.SetPosition(transformComponent["Position"].as<Vec3>());
					tc.SetRotation(transformComponent["Rotation"].as<Vec3>());
					tc.SetScale(transformComponent["Scale"].as<Vec3>());
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();

					auto& cameraProps = cameraComponent["Camera"];
					if (cameraProps["ProjectionType"])
						cc.GetCamera().SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

					if (cameraProps["PerspectiveFOV"])
						cc.GetCamera().SetPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as<float>());
					if (cameraProps["PerspectiveNear"])
						cc.GetCamera().SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
					if (cameraProps["PerspectiveFar"])
						cc.GetCamera().SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

					if (cameraProps["OrthographicSize"])
						cc.GetCamera().SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					if (cameraProps["OrthographicNear"])
						cc.GetCamera().SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
					if (cameraProps["OrthographicFar"])
						cc.GetCamera().SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());
					if (cameraProps["PixelPerfect"])
						cc.GetCamera().SetPixelPerfect(cameraProps["PixelPerfect"].as<bool>());

					if (cameraProps["Primary"])
						cc.SetIsPrimaryCamera(cameraComponent["Primary"].as<bool>());
					if (cameraProps["FixedAspectRatio"])
						cc.SetHasFixedAspectRatio(cameraComponent["FixedAspectRatio"].as<bool>());
				}

				auto scriptComponent = entity["ScriptComponent"];
				if (scriptComponent)
				{
					auto& sc = deserializedEntity.AddComponent<ScriptComponent>();
					sc.m_scriptName = scriptComponent["ScriptName"].as<std::string>();
				}

				auto spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent)
				{
					auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
					src.GetColor() = spriteRendererComponent["Color"].as<Vec4>();
					if (spriteRendererComponent["Texture"].Type() != YAML::NodeType::Undefined)
					{
						std::string texturePath = spriteRendererComponent["Texture"].as<std::string>();
						auto path = Project::GetAssetFileSystemPath(texturePath);
						src.m_texture = Texture2D::Create(path.string());

						src.SetRows(spriteRendererComponent["Rows"].as<int>());
						src.SetColumns(spriteRendererComponent["Columns"].as<int>());
						src.SetPadding(spriteRendererComponent["Padding"].as<int>());
						src.SetFrame(spriteRendererComponent["Frame"].as<int>());
					}
				}

				auto circleRendererComponent = entity["CircleRendererComponent"];
				if (circleRendererComponent)
				{
					auto& crc = deserializedEntity.AddComponent<CircleRendererComponent>();
					crc.m_color = circleRendererComponent["Color"].as<Vec4>();
					crc.m_thickness = circleRendererComponent["Thickness"].as<float>();
					crc.m_fade = circleRendererComponent["Fade"].as<float>();
				}

				auto rigidbody2DComponent = entity["Rigidbody2DComponent"];
				if (rigidbody2DComponent)
				{
					auto& rb = deserializedEntity.AddComponent<Rigidbody2DComponent>();
					rb.m_type = RigidBody2DBodyTypeFromString(rigidbody2DComponent["BodyType"].as<std::string>());
					rb.m_fixedRotation = rigidbody2DComponent["FixedRotation"].as<bool>();
				}

				auto pixelPlatformerBodyComponent = entity["PixelPlatformerBodyComponent"];
				if (pixelPlatformerBodyComponent)
				{
					auto& ppb = deserializedEntity.AddComponent<PixelPlatformerBodyComponent>();
					ppb.m_type = PixelPlatformerBodyTypeFromString(pixelPlatformerBodyComponent["BodyType"].as<std::string>());
					ppb.m_fixedRotation = pixelPlatformerBodyComponent["FixedRotation"].as<bool>();
				}

				auto boxCollider2DComponent = entity["BoxCollider2DComponent"];
				if (boxCollider2DComponent)
				{
					auto& coll = deserializedEntity.AddComponent<BoxCollider2DComponent>();
					coll.m_offset = boxCollider2DComponent["Offset"].as<Vec2>();
					coll.m_size = boxCollider2DComponent["Size"].as<Vec2>();
					coll.m_density = boxCollider2DComponent["Density"].as<float>();
					coll.m_friction = boxCollider2DComponent["Friction"].as<float>();
					coll.m_restitution = boxCollider2DComponent["Restitution"].as<float>();
					coll.m_restitutionThreshold = boxCollider2DComponent["RestitutionThreshold"].as<float>();
				}

				auto circleCollider2DComponent = entity["CircleCollider2DComponent"];
				if (circleCollider2DComponent)
				{
					auto& coll = deserializedEntity.AddComponent<CircleCollider2DComponent>();
					coll.m_offset = circleCollider2DComponent["Offset"].as<Vec2>();
					coll.m_radius = circleCollider2DComponent["Radius"].as<float>();
					coll.m_density = circleCollider2DComponent["Density"].as<float>();
					coll.m_friction = circleCollider2DComponent["Friction"].as<float>();
					coll.m_restitution = circleCollider2DComponent["Restitution"].as<float>();
					coll.m_restitutionThreshold = circleCollider2DComponent["RestitutionThreshold"].as<float>();
				}

				auto boxArea2DComponent = entity["BoxArea2DComponent"];
				if (boxArea2DComponent)
				{
					auto& coll = deserializedEntity.AddComponent<BoxArea2DComponent>();
					coll.m_offset = boxArea2DComponent["Offset"].as<Vec2>();
					coll.m_size = boxArea2DComponent["Size"].as<Vec2>();
					coll.GetDebugColor() = boxArea2DComponent["DebugColor"].as<Vec4>();
				}

				auto tileMapComponent = entity["TileMapComponent"];
				if (tileMapComponent)
				{
					auto& tilemap = deserializedEntity.AddComponent<TileMapComponent>();
					if (tileMapComponent["TileMap"].Type() != YAML::NodeType::Undefined)
					{
						tilemap.m_tilemap = TileSerializer::DeserializeTileMap(tileMapComponent["TileMap"].as<std::string>());
					}
				}

				auto platformerPlayerControllerComponent = entity["PlatformerPlayerControllerComponent"];
				if (platformerPlayerControllerComponent)
				{
					auto& controller = deserializedEntity.AddComponent<PlatformerPlayerControllerComponent>();
					controller.m_speed = platformerPlayerControllerComponent["Speed"].as<float>();
					controller.m_jumpHeight = platformerPlayerControllerComponent["JumpHeight"].as<float>();
				}

				auto animtorComponent = entity["AnimatorComponent"];
				if (animtorComponent)
				{
					auto& sc = deserializedEntity.AddComponent<AnimatorComponent>();
					sc.m_filePath = animtorComponent["FilePath"].as<std::string>();
					auto path = Project::GetAssetFileSystemPath(sc.m_filePath);
					AnimationSerializer::DeserializeAnimations(path.string(), deserializedEntity);
				}

				m_scene->DeserializeEntity(&entity, deserializedEntity);
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		// Not implemented
		Log::Assert(false, "");
		return false;
	}
}