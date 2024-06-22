#include "PatienceScene.h"

#include "Components/CardComponent.h"
#include "Components/CardSlotComponent.h"
#include "Rhombus.h"

#include <fstream>

#define YAML_CPP_STATIC_DEFINE		// Needed for yaml static library to work for some reason
#include <yaml-cpp/yaml.h>

namespace YAML {

	template<>
	struct convert<rhombus::Vec2>
	{
		static Node encode(const rhombus::Vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, rhombus::Vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<rhombus::Vec3>
	{
		static Node encode(const rhombus::Vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, rhombus::Vec3& rhs)
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
	struct convert<rhombus::Vec4>
	{
		static Node encode(const rhombus::Vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, rhombus::Vec4& rhs)
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
	// Update this list when any new components are added
	// -----------------------------------------------------
	using PatienceComponents =
		ComponentGroup<CardComponent, CardSlotComponent>;
	// -----------------------------------------------------

	PatienceScene::PatienceScene()
	{
		InitScene();
	}

	void PatienceScene::InitScene()
	{
		Scene::InitScene();
		RegisterComponents(PatienceComponents{});

		cardPlacementSystem = m_Registry.RegisterSystem<CardPlacementSystem>(this);
		{
			Signature signature;
			signature.set(m_Registry.GetComponentType<CardComponent>());
			m_Registry.SetSystemSignature<CardPlacementSystem>(signature);
		}
	}

	void PatienceScene::OnRuntimeStart()
	{
		Scene::OnRuntimeStart();
		cardPlacementSystem->Init();
	}

	void PatienceScene::OnUpdateRuntime(DeltaTime dt)
	{
		Scene::OnUpdateRuntime(dt);
	}

	void PatienceScene::CopyAllComponents(Ref<Scene> destScene, const std::unordered_map<UUID, EntityID>& entityMap)
	{
		Scene::CopyAllComponents(destScene, entityMap);
		CopyComponent(PatienceComponents{}, destScene, m_Registry, entityMap);
	}

	void PatienceScene::CopyEntityComponents(Entity dest, Entity src)
	{
		Scene::CopyEntityComponents(dest, src);
		CopyComponentIfExists(PatienceComponents{}, dest, src);
	}

	void PatienceScene::SerializeEntity(void* yamlEmitter, Entity entity)
	{
		YAML::Emitter& out = *(YAML::Emitter*)yamlEmitter;
		if (entity.HasComponent<CardComponent>())
		{
			out << YAML::Key << "CardComponent";
			out << YAML::BeginMap; // CardComponent

			auto& cardComponent = entity.GetComponent<CardComponent>();
			out << YAML::Key << "Rank" << YAML::Value << cardComponent.m_rank;
			out << YAML::Key << "Suit" << YAML::Value << cardComponent.m_suit;

			out << YAML::EndMap; // CardComponent
		}

		if (entity.HasComponent<CardSlotComponent>())
		{
			out << YAML::Key << "CardSlotComponent";
			out << YAML::BeginMap; // CardSlotComponent

			auto& cardComponent = entity.GetComponent<CardSlotComponent>();
			out << YAML::Key << "SlotType" << YAML::Value << (int)cardComponent.GetSlotType();

			// out << YAML::Key << "StaggeredOffset" << YAML::Value << cardComponent.m_staggeredOffset;
			// TODO: FIX THIS! Why aren't the YAML coverts from SceneSerializer working?
			const Vec2 vec2Encoding = cardComponent.GetStaggeredOffset();
			YAML::Node node;
			node.push_back(vec2Encoding.x);
			node.push_back(vec2Encoding.y);
			node.SetStyle(YAML::EmitterStyle::Flow);
			out << YAML::Key << "StaggeredOffset" << YAML::Value << node;

			out << YAML::EndMap; // CardSlotComponent
		}
	}

	void PatienceScene::DeserializeEntity(void* yamlEntity, Entity entity)
	{
		YAML::Node node = *(YAML::Node*)yamlEntity;

		auto cardComponent = node["CardComponent"];
		if (cardComponent)
		{
			auto& card = entity.AddComponent<CardComponent>();
			card.m_rank = cardComponent["Rank"].as<int>();
			card.m_suit = cardComponent["Suit"].as<int>();
		}

		auto cardSlotComponent = node["CardSlotComponent"];
		if (cardSlotComponent)
		{
			auto& cardSlot = entity.AddComponent<CardSlotComponent>();
			cardSlot.SetSlotType(cardSlotComponent["SlotType"].as<int>());
			cardSlot.SetStaggeredOffset(cardSlotComponent["StaggeredOffset"].as<Vec2>());
		}
	}

	void PatienceScene::OnMouseMoved(int x, int y)
	{
		Scene::OnMouseMoved(x, y);

		cardPlacementSystem->OnMouseMoved(x, y);
	}

	void PatienceScene::OnMouseButtonPressed(int button)
	{
		Scene::OnMouseButtonPressed(button);

		cardPlacementSystem->OnMouseButtonPressed(button);
	}

	void PatienceScene::OnMouseButtonReleased(int button)
	{
		Scene::OnMouseButtonReleased(button);

		cardPlacementSystem->OnMouseButtonReleased(button);
	}
}