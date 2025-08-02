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
			out << YAML::Key << "DoubleJumpHeight" << YAML::Value << controller.m_doubleJumpHeight;

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

		// Save other files
		for (EntityID e : orderedEntities)
		{
			Entity entity = { e, m_scene.get() };
			if (!entity)
				return;

			if (entity.HasComponent<AnimatorComponent>())
			{
				auto& animatorComponent = entity.GetComponent<AnimatorComponent>();
				AnimationSerializer::SerializeAnimations(Project::GetAssetFileSystemPath(animatorComponent.m_filePath).string(), entity);
			}
		}
	}

	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
		// Not implemented
		Log::Assert(false, "");
	}

	template<typename T>
	void DeserializeData(std::string value, YAML::Node node, T& output)
	{
		if (node[value].IsDefined())
		{
			output = node[value].as<T>();
		}
	}

	template<typename T>
	void DeserializeData(std::string value, YAML::Node node, std::function<void(T)> f)
	{
		if (node[value].IsDefined())
		{
			f(node[value].as<T>());
		}
	}

	template<typename T>
	void DeserializeDataEnum(std::string value, YAML::Node node, T& output, std::function<T(const std::string&)> f)
	{
		if (node[value].IsDefined())
		{
			output = f(node[value].as<std::string>());
		}
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
					DeserializeData<Vec3>("Position", transformComponent, [&tc](Vec3 val) { tc.SetPosition(val); });
					DeserializeData<Vec3>("Rotation", transformComponent, [&tc](Vec3 val) { tc.SetRotation(val); });
					DeserializeData<Vec3>("Scale", transformComponent, [&tc](Vec3 val) { tc.SetScale(val); });
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();

					auto& cameraProps = cameraComponent["Camera"];
					DeserializeData<int>("ProjectionType", cameraProps, [&cc](int val) { cc.GetCamera().SetProjectionType((SceneCamera::ProjectionType)val); });

					DeserializeData<float>("PerspectiveFOV", cameraProps, [&cc](float val) { cc.GetCamera().SetPerspectiveVerticalFOV(val); });
					DeserializeData<float>("PerspectiveNear", cameraProps, [&cc](float val) { cc.GetCamera().SetPerspectiveNearClip(val); });
					DeserializeData<float>("PerspectiveFar", cameraProps, [&cc](float val) { cc.GetCamera().SetPerspectiveFarClip(val); });

					DeserializeData<float>("OrthographicSize", cameraProps, [&cc](float val) { cc.GetCamera().SetOrthographicSize(val); });
					DeserializeData<float>("OrthographicNear", cameraProps, [&cc](float val) { cc.GetCamera().SetOrthographicNearClip(val); });
					DeserializeData<float>("OrthographicFar", cameraProps, [&cc](float val) { cc.GetCamera().SetOrthographicFarClip(val); });
					DeserializeData<bool>("PixelPerfect", cameraProps, [&cc](bool val) { cc.GetCamera().SetPixelPerfect(val); });

					DeserializeData<bool>("Primary", cameraComponent, [&cc](bool val) { cc.SetIsPrimaryCamera(val); });
					DeserializeData<bool>("FixedAspectRatio", cameraComponent, [&cc](bool val) { cc.SetHasFixedAspectRatio(val); });
				}

				auto scriptComponent = entity["ScriptComponent"];
				if (scriptComponent)
				{
					auto& sc = deserializedEntity.AddComponent<ScriptComponent>();
					DeserializeData<std::string>("ScriptName", scriptComponent, sc.m_scriptName);
				}

				auto spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent)
				{
					auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
					DeserializeData<Color>("Color", spriteRendererComponent, src.GetColor());
					if (spriteRendererComponent["Texture"].Type() != YAML::NodeType::Undefined)
					{
						std::string texturePath;
						DeserializeData<std::string>("Texture", spriteRendererComponent, texturePath);
						auto path = Project::GetAssetFileSystemPath(texturePath);
						src.m_texture = Texture2D::Create(path.string());
						DeserializeData<int>("Rows", spriteRendererComponent, [&src](int val) { src.SetRows(val); });
						DeserializeData<int>("Columns", spriteRendererComponent, [&src](int val) { src.SetColumns(val); });
						DeserializeData<int>("Padding", spriteRendererComponent, [&src](int val) { src.SetPadding(val); });
						DeserializeData<int>("Frame", spriteRendererComponent, [&src](int val) { src.SetFrame(val); });
					}
				}

				auto circleRendererComponent = entity["CircleRendererComponent"];
				if (circleRendererComponent)
				{
					auto& crc = deserializedEntity.AddComponent<CircleRendererComponent>();
					DeserializeData<Color>("Color", circleRendererComponent, crc.m_color);
					DeserializeData<float>("Thickness", circleRendererComponent, crc.m_thickness);
					DeserializeData<float>("Fade", circleRendererComponent, crc.m_fade);
				}

				auto rigidbody2DComponent = entity["Rigidbody2DComponent"];
				if (rigidbody2DComponent)
				{
					auto& rb = deserializedEntity.AddComponent<Rigidbody2DComponent>();
					DeserializeDataEnum<Rigidbody2DComponent::BodyType>("BodyType", rigidbody2DComponent, rb.m_type, &RigidBody2DBodyTypeFromString);
					DeserializeData<bool>("FixedRotation", rigidbody2DComponent, rb.m_fixedRotation);
				}

				auto pixelPlatformerBodyComponent = entity["PixelPlatformerBodyComponent"];
				if (pixelPlatformerBodyComponent)
				{
					auto& ppb = deserializedEntity.AddComponent<PixelPlatformerBodyComponent>();
					DeserializeDataEnum<PixelPlatformerBodyComponent::BodyType>("BodyType", pixelPlatformerBodyComponent, ppb.m_type, &PixelPlatformerBodyTypeFromString);
					DeserializeData<bool>("FixedRotation", pixelPlatformerBodyComponent, ppb.m_fixedRotation);
				}

				auto boxCollider2DComponent = entity["BoxCollider2DComponent"];
				if (boxCollider2DComponent)
				{
					auto& coll = deserializedEntity.AddComponent<BoxCollider2DComponent>();
					DeserializeData<Vec2>("Offset", boxCollider2DComponent, coll.m_offset);
					DeserializeData<Vec2>("Size", boxCollider2DComponent, coll.m_size);
					DeserializeData<float>("Density", boxCollider2DComponent, coll.m_density);
					DeserializeData<float>("Friction", boxCollider2DComponent, coll.m_friction);
					DeserializeData<float>("Restitution", boxCollider2DComponent, coll.m_restitution);
					DeserializeData<float>("RestitutionThreshold", boxCollider2DComponent, coll.m_restitutionThreshold);
				}

				auto circleCollider2DComponent = entity["CircleCollider2DComponent"];
				if (circleCollider2DComponent)
				{
					auto& coll = deserializedEntity.AddComponent<CircleCollider2DComponent>();
					DeserializeData<Vec2>("Offset", circleCollider2DComponent, coll.m_offset);
					DeserializeData<float>("Radius", circleCollider2DComponent, coll.m_radius);
					DeserializeData<float>("Density", circleCollider2DComponent, coll.m_density);
					DeserializeData<float>("Friction", circleCollider2DComponent, coll.m_friction);
					DeserializeData<float>("Restitution", circleCollider2DComponent, coll.m_restitution);
					DeserializeData<float>("RestitutionThreshold", circleCollider2DComponent, coll.m_restitutionThreshold);
				}

				auto boxArea2DComponent = entity["BoxArea2DComponent"];
				if (boxArea2DComponent)
				{
					auto& coll = deserializedEntity.AddComponent<BoxArea2DComponent>();
					DeserializeData<Vec2>("Offset", boxArea2DComponent, coll.m_offset);
					DeserializeData<Vec2>("Size", boxArea2DComponent, coll.m_size);
					DeserializeData<Color>("DebugColor", boxArea2DComponent, coll.GetDebugColor());
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
					DeserializeData<float>("Speed", platformerPlayerControllerComponent, controller.m_speed);
					DeserializeData<float>("JumpHeight", platformerPlayerControllerComponent, controller.m_jumpHeight);
					DeserializeData<float>("DoubleJumpHeight", platformerPlayerControllerComponent, controller.m_doubleJumpHeight);
				}

				auto animtorComponent = entity["AnimatorComponent"];
				if (animtorComponent)
				{
					auto& animator = deserializedEntity.AddComponent<AnimatorComponent>();
					DeserializeData<std::string>("FilePath", animtorComponent, animator.m_filePath);
					auto path = Project::GetAssetFileSystemPath(animator.m_filePath);
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