#include "PatienceScene.h"

#include "Components/CardComponent.h"
#include "Components/CardSlotComponent.h"
#include "Components/PatienceManagerComponent.h"
#include "Rhombus/ECS/SceneSerializer.h"
#include "Rhombus.h"

namespace rhombus
{
	// Update this list when any new components are added
	// -----------------------------------------------------
	using PatienceComponents =
		ComponentGroup<CardComponent, CardSlotComponent, PatienceManagerComponent>;
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

			out << YAML::Key << "StaggeredOffset" << YAML::Value << cardComponent.m_staggeredOffset;

			out << YAML::EndMap; // CardSlotComponent
		}

		if (entity.HasComponent<PatienceManagerComponent>())
		{
			auto& managerComponent = entity.GetComponent<PatienceManagerComponent>();

			out << YAML::Key << "PatienceManagerComponent";
			out << YAML::BeginMap;	// PatienceManagerComponent
			out << YAML::Key << "SetupScript" << YAML::Value << managerComponent.m_setupScript;
			out << YAML::EndMap;	// PatienceManagerComponent
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

		auto managerComponent = node["PatienceManagerComponent"];
		if (managerComponent)
		{
			auto& manager = entity.AddComponent<PatienceManagerComponent>();
			manager.m_setupScript = managerComponent["SetupScript"].as<std::string>();
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