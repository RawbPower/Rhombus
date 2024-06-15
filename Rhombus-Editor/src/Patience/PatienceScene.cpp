#include "PatienceScene.h"

#include "Components/CardComponent.h"
#include "Components/CardSlotComponent.h"
#include "Rhombus.h"

#include <fstream>

#define YAML_CPP_STATIC_DEFINE		// Needed for yaml static library to work for some reason
#include <yaml-cpp/yaml.h>

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
	}

	void PatienceScene::OnUpdateRuntime(DeltaTime dt)
	{
		Scene::OnUpdateRuntime(dt);
	}

	void PatienceScene::CopyAllComponents(Ref<Scene> destScene, const std::unordered_map<UUID, EntityID>& entityMap)
	{
		Scene::CopyAllComponents(destScene, entityMap);
		CopyComponent(PatienceComponents{}, destScene->GetRegistry(), m_Registry, entityMap);
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
			auto& card = entity.AddComponent<CardSlotComponent>();
		}
	}

	void PatienceScene::OnMouseMoved(int x, int y)
	{
		Scene::OnMouseMoved(x, y);
		Vec3 cursorCoords = Renderer2D::ConvertScreenToWorldSpace(x, y);

		{
			std::vector<EntityID> view = m_Registry.GetEntityList<CardComponent>();
			for (EntityID e : view)
			{
				Entity entity = { e, this };
				auto& transform = entity.GetComponent<TransformComponent>();
				auto& card = entity.GetComponent<CardComponent>();

				if (card.GetIsHeld())
				{
					transform.m_position = Vec3(cursorCoords.x, cursorCoords.y, transform.m_position.z);
				}
			}
		}
	}

	void PatienceScene::OnMouseButtonPressed(int button)
	{
		Scene::OnMouseButtonPressed(button);

		{
			std::vector<EntityID> view = m_Registry.GetEntityList<CardComponent>();
			for (EntityID e : view)
			{
				Entity entity = { e, this };
				auto& card = entity.GetComponent<CardComponent>();

				if (entity.HasComponent<BoxArea2DComponent>())
				{
					auto& ba2D = entity.GetComponent<BoxArea2DComponent>();

					if (ba2D.m_isMouseInArea)
					{
						auto& transform = entity.GetComponent<TransformComponent>();
						card.SetIsHeld(true);
						card.SetPreviousPosition(transform.m_position);
					}
				}
			}
		}
	}

	void PatienceScene::OnMouseButtonReleased(int button)
	{
		Scene::OnMouseButtonReleased(button);

		{
			std::vector<EntityID> view = m_Registry.GetEntityList<CardComponent>();
			for (EntityID e : view)
			{
				Entity entity = { e, this };
				auto& card = entity.GetComponent<CardComponent>();

				if (card.GetIsHeld())
				{
					card.SetIsHeld(false);
					PlaceCard(entity);
				}
			}
		}
	}

	void PatienceScene::PlaceCard(Entity cardEntity)
	{
		const CardComponent& card = cardEntity.GetComponent<CardComponent>();
		const BoxArea2DComponent& cardBoxArea = cardEntity.GetComponent<BoxArea2DComponent>();
		TransformComponent& cardTransform = cardEntity.GetComponent<TransformComponent>();
		EntityID nearestCardSlot = -1;
		float nearestSeparation = -1.0f;
		std::vector<EntityID> view = m_Registry.GetEntityList<CardSlotComponent>();
		for (EntityID e : view)
		{
			Entity entity = { e, this };
			const BoxArea2DComponent& slotArea = entity.GetComponent<BoxArea2DComponent>();
			const TransformComponent& slotTransform = entity.GetComponent<TransformComponent>();

			Vec3 cardPos = cardTransform.m_position + Vec3(cardBoxArea.m_offset, 0.0f);
			Vec3 slotPos = slotTransform.m_position + Vec3(slotArea.m_offset, 0.0f);

			Vec2 separation = Vec2(abs(cardPos.x - slotPos.x), abs(cardPos.y - slotPos.y));
			if (separation.GetMagnitude() < nearestSeparation || nearestSeparation < 0.0f)
			{
				nearestCardSlot = entity;
				nearestSeparation = separation.GetMagnitude();
			}
		}

		bool placedInSlot = false;
		if (nearestCardSlot >= 0)
		{
			Entity entity = { nearestCardSlot, this };
			const BoxArea2DComponent& slotArea = entity.GetComponent<BoxArea2DComponent>();
			const TransformComponent& slotTransform = entity.GetComponent<TransformComponent>();

			Vec3 cardPos = cardTransform.m_position + Vec3(cardBoxArea.m_offset, 0.0f);
			Vec3 slotPos = slotTransform.m_position + Vec3(slotArea.m_offset, 0.0f);

			Vec2 separation = Vec2(abs(cardPos.x - slotPos.x), abs(cardPos.y - slotPos.y));

			if (separation.x < (slotArea.m_size.x + cardBoxArea.m_size.x) && separation.y < (slotArea.m_size.y + cardBoxArea.m_size.y))
			{
				cardTransform.m_position = slotTransform.m_position;
				placedInSlot = true;
			}
		}

		if (!placedInSlot)
		{
			cardTransform.m_position = Vec3(card.GetPreviousPosition(), cardTransform.m_position.z);
		}
	}

	template<>
	void Scene::OnComponentAdded<CardComponent>(Entity entity, CardComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CardSlotComponent>(Entity entity, CardSlotComponent& component)
	{
	}
}