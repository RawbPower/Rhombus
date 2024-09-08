#include "CardSlotSystem.h"

#include "Components/CardSlotComponent.h"
#include "Components/PatienceComponent.h"
#include "Rhombus/ECS/Components/Area2DComponent.h"

#include <algorithm>
#include <random>

void CardSlotSystem::Init()
{
	Entity patienceEntity = { m_scene->GetRegistry().GetFirstEntity<PatienceComponent>(), m_scene };
	PatienceComponent& patienceComponent = patienceEntity.GetComponent<PatienceComponent>();
	for (Entity entity : GetEntities())
	{
		auto& cardSlot = entity.GetComponent<CardSlotComponent>();

		entity.GetComponent<TransformComponent>().SetLayer(Z_LAYER::MIDDLEGROUND_LAYER);

		if (cardSlot.GetSlotType() == CardSlotComponent::SlotType::SLOT_TYPE_MONSTER)
		{
			patienceComponent.m_monsterSlots.push_back(entity);
			m_scene->GetEntityByUUID(cardSlot.m_monsterBattleSite).GetComponent<CardSlotComponent>().m_monsterSlot = entity;
		}

		if (entity.HasComponent<BoxArea2DComponent>())
		{
			BoxArea2DComponent& boxArea = entity.GetComponent<BoxArea2DComponent>();
			cardSlot.m_emptyAreaOffset = boxArea.m_offset;
			cardSlot.m_emptyAreaSize = boxArea.m_size;
		}

		cardSlot.UpdateAllowedCards();
	}
}
