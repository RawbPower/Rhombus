#include "rbpch.h"
#include "SceneSerializer.h"

#include "Entity.h"

#include "Rhombus/Project/Project.h"

#include <fstream>

#define YAML_CPP_STATIC_DEFINE		// Needed for yaml static library to work for some reason
#include <yaml-cpp/yaml.h>

namespace YAML {

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

}

namespace rhombus
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	static std::string RigidBody2DBodyTypeToString(Rigidbody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
		case Rigidbody2DComponent::BodyType::Static:    return "Static";
		case Rigidbody2DComponent::BodyType::Dynamic:   return "Dynamic";
		case Rigidbody2DComponent::BodyType::Kinematic: return "Kinematic";
		}

		RB_CORE_ASSERT(false, "Unknown body type");
		return {};
	}

	static Rigidbody2DComponent::BodyType RigidBody2DBodyTypeFromString(const std::string& bodyTypeString)
	{
		if (bodyTypeString == "Static")    return Rigidbody2DComponent::BodyType::Static;
		if (bodyTypeString == "Dynamic")   return Rigidbody2DComponent::BodyType::Dynamic;
		if (bodyTypeString == "Kinematic") return Rigidbody2DComponent::BodyType::Kinematic;

		RB_CORE_ASSERT(false, "Unknown body type");
		return Rigidbody2DComponent::BodyType::Static;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: m_scene(scene)
	{
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity, Ref<Scene>& scene)
	{
		RB_CORE_ASSERT(entity.HasComponent<IDComponent>(), "Entity does not have a UUID");

		out << YAML::BeginMap; // Entity
		out << YAML::Key << "Entity" << YAML::Value << entity.GetUUID();

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
			out << YAML::Key << "Position" << YAML::Value << tc.m_position;
			out << YAML::Key << "Rotation" << YAML::Value << tc.m_rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.m_scale;

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

			out << YAML::EndMap; // BoxArea2DComponent
		}

		scene->SerializeEntity(&out, entity);

		out << YAML::EndMap; // Entity
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		std::vector<EntityID> entityList = m_scene->GetAllEntities();
		for (EntityID e : entityList)
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
		RB_CORE_ASSERT(false, "");
	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{ 
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(filepath);
		}
		catch (YAML::ParserException e)
		{
			RB_CORE_ERROR("Failed to load .hazel file '{0}'\n     {1}", filepath, e.what());
			return false;
		}

		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		RB_CORE_TRACE("Deserializing scene '{0}'", sceneName);

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>();

				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				RB_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);

				Entity deserializedEntity = m_scene->CreateEntityWithUUID(uuid, name);

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					// Entities always have transforms
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.m_position = transformComponent["Position"].as<glm::vec3>();
					tc.m_rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.m_scale = transformComponent["Scale"].as<glm::vec3>();
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
					src.GetColor() = spriteRendererComponent["Color"].as<glm::vec4>();
					if (spriteRendererComponent["Texture"].Type() != YAML::NodeType::Undefined)
					{
						std::string texturePath = spriteRendererComponent["Texture"].as<std::string>();
						auto path = Project::GetAssetFileSystemPath(texturePath);
						src.m_texture = Texture2D::Create(path.string());
					}
				}

				auto circleRendererComponent = entity["CircleRendererComponent"];
				if (circleRendererComponent)
				{
					auto& crc = deserializedEntity.AddComponent<CircleRendererComponent>();
					crc.m_color = circleRendererComponent["Color"].as<glm::vec4>();
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

				auto boxCollider2DComponent = entity["BoxCollider2DComponent"];
				if (boxCollider2DComponent)
				{
					auto& coll = deserializedEntity.AddComponent<BoxCollider2DComponent>();
					coll.m_offset = boxCollider2DComponent["Offset"].as<glm::vec2>();
					coll.m_size = boxCollider2DComponent["Size"].as<glm::vec2>();
					coll.m_density = boxCollider2DComponent["Density"].as<float>();
					coll.m_friction = boxCollider2DComponent["Friction"].as<float>();
					coll.m_restitution = boxCollider2DComponent["Restitution"].as<float>();
					coll.m_restitutionThreshold = boxCollider2DComponent["RestitutionThreshold"].as<float>();
				}

				auto circleCollider2DComponent = entity["CircleCollider2DComponent"];
				if (circleCollider2DComponent)
				{
					auto& coll = deserializedEntity.AddComponent<CircleCollider2DComponent>();
					coll.m_offset = circleCollider2DComponent["Offset"].as<glm::vec2>();
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
					coll.m_offset = boxArea2DComponent["Offset"].as<glm::vec2>();
					coll.m_size = boxArea2DComponent["Size"].as<glm::vec2>();
				}

				m_scene->DeserializeEntity(&entity, deserializedEntity);
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		// Not implemented
		RB_CORE_ASSERT(false, "");
		return false;
	}
}