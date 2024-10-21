#include "PatienceScene.h"

#include "Components/CardComponent.h"
#include "Components/CardSlotComponent.h"
#include "Components/PatienceComponent.h"
#include "Rhombus/Scenes/SceneSerializer.h"

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

	cardSlotSystem = m_Registry.RegisterSystem<CardSlotSystem>(this);
	{
		Signature signature;
		signature.set(m_Registry.GetComponentType<CardSlotComponent>());
		m_Registry.SetSystemSignature<CardSlotSystem>(signature);
	}

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
	setupSystem->Init();
	cardSlotSystem->Init();
	cardPlacementSystem->Init();
}

void PatienceScene::OnUpdateRuntime(DeltaTime dt)
{
	Scene::OnUpdateRuntime(dt);
}

void PatienceScene::OnDraw()
{
	std::vector<EntityID> view = GetAllEntitiesWith<BoxArea2DComponent>();
	for (EntityID e : view)
	{
		Entity entity = { e, this };
		TransformComponent& tc = entity.GetComponent<TransformComponent>();
		BoxArea2DComponent& ba2d = entity.GetComponent<BoxArea2DComponent>();
		if (entity.HasComponent<CardComponent>())
		{
			CardComponent& card = entity.GetComponent<CardComponent>();
			if (card.GetIsHovered())
			{
				Vec3 translation = tc.GetWorldPosition() + Vec3(ba2d.m_offset, 0.01f);
				Vec3 scale = tc.GetWorldScale() * Vec3(ba2d.m_size * 2.0f, 1.0f);

				Mat4 transform = math::Translate(Mat4::Identity(), translation)
					* math::Rotate(Mat4::Identity(), tc.GetWorldRotation().z, Vec3(0.0f, 0.0f, 1.0f))
					* math::Scale(Mat4::Identity(), scale);
				Renderer2D::DrawRect(transform, Color(0.0f, 0.8f, 0.1f, 1.0f));
			}
		}
	}
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

		auto& cardSlotComponent = entity.GetComponent<CardSlotComponent>();
		out << YAML::Key << "SlotType" << YAML::Value << (int)cardSlotComponent.GetSlotType();
		out << YAML::Key << "SlotLayout" << YAML::Value << (int)cardSlotComponent.GetSlotLayout();

		out << YAML::Key << "StaggeredOffset" << YAML::Value << cardSlotComponent.m_staggeredOffset;

		out << YAML::Key << "MonsterBattleSite" << YAML::Value << cardSlotComponent.m_monsterBattleSite;

		out << YAML::Key << "Site:FoundationSuit" << YAML::Value << cardSlotComponent.m_siteInfo.m_foundationSuit;
		out << YAML::Key << "Site:FoundationRank" << YAML::Value << cardSlotComponent.m_siteInfo.m_foundationRank;
		out << YAML::Key << "Site:RankOrdering" << YAML::Value << (int)cardSlotComponent.m_siteInfo.m_rankOrdering;
		out << YAML::Key << "Site:SuitOrdering" << YAML::Value << (int)cardSlotComponent.m_siteInfo.m_suitOrdering;
		out << YAML::Key << "Site:CanLoop" << YAML::Value << cardSlotComponent.m_siteInfo.m_canLoop;
		out << YAML::Key << "Site:LoopMax" << YAML::Value << cardSlotComponent.m_siteInfo.m_loopMax;

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
		cardSlot.SetSlotLayout(cardSlotComponent["SlotLayout"].as<int>());
		cardSlot.SetStaggeredOffset(cardSlotComponent["StaggeredOffset"].as<Vec2>());
		cardSlot.m_monsterBattleSite = cardSlotComponent["MonsterBattleSite"].as<uint64_t>();

		cardSlot.m_siteInfo.m_foundationSuit = cardSlotComponent["Site:FoundationSuit"].as<uint32_t>();
		cardSlot.m_siteInfo.m_foundationRank = cardSlotComponent["Site:FoundationRank"].as<int>();
		cardSlot.m_siteInfo.m_rankOrdering = (CardSlotComponent::RankOrdering)cardSlotComponent["Site:RankOrdering"].as<int>();
		cardSlot.m_siteInfo.m_suitOrdering = (CardSlotComponent::SuitOrdering)cardSlotComponent["Site:SuitOrdering"].as<int>();
		cardSlot.m_siteInfo.m_canLoop = cardSlotComponent["Site:CanLoop"].as<bool>();
		cardSlot.m_siteInfo.m_loopMax = cardSlotComponent["Site:LoopMax"].as<int>();
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