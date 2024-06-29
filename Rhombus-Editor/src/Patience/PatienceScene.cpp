#include "PatienceScene.h"

#include "Components/CardComponent.h"
#include "Components/CardSlotComponent.h"
#include "Components/PatienceComponent.h"
#include "Rhombus/ECS/SceneSerializer.h"

// Update this list when any new components are added
// -----------------------------------------------------
using PatienceComponents =
	ComponentGroup<CardComponent, CardSlotComponent, PatienceComponent>;
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

	setupSystem = m_Registry.RegisterSystem<PatienceSetupSystem>(this);
	{
		Signature signature;
		signature.set(m_Registry.GetComponentType<PatienceComponent>());
		m_Registry.SetSystemSignature<PatienceSetupSystem>(signature);
	}

}

void PatienceScene::OnRuntimeStart()
{
	Scene::OnRuntimeStart();
	cardPlacementSystem->Init();
	setupSystem->Init();
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

	if (entity.HasComponent<PatienceComponent>())
	{
		auto& patienceComponent = entity.GetComponent<PatienceComponent>();

		out << YAML::Key << "PatienceComponent";
		out << YAML::BeginMap;	// PatienceComponent
		out << YAML::Key << "SetupScript" << YAML::Value << patienceComponent.m_setupScript;
		out << YAML::EndMap;	// PatienceComponent
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
		card.m_suit = (CardComponent::Suit)cardComponent["Suit"].as<int>();
	}

	auto cardSlotComponent = node["CardSlotComponent"];
	if (cardSlotComponent)
	{
		auto& cardSlot = entity.AddComponent<CardSlotComponent>();
		cardSlot.SetSlotType(cardSlotComponent["SlotType"].as<int>());
		cardSlot.SetStaggeredOffset(cardSlotComponent["StaggeredOffset"].as<Vec2>());
	}

	auto patienceComponent = node["PatienceComponent"];
	if (patienceComponent)
	{
		auto& patience = entity.AddComponent<PatienceComponent>();
		patience.m_setupScript = patienceComponent["SetupScript"].as<std::string>();
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