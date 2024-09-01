#include "CardSlotSystem.h"

#include "Components/CardSlotComponent.h"
#include "Rhombus/ECS/Components/Area2DComponent.h"

#include <algorithm>
#include <random>

void CardSlotSystem::Init()
{
	for (Entity entity : GetEntities())
	{
		auto& cardSlot = entity.GetComponent<CardSlotComponent>();
		if (entity.HasComponent<BoxArea2DComponent>())
		{
			BoxArea2DComponent& boxArea = entity.GetComponent<BoxArea2DComponent>();
			cardSlot.m_emptyAreaOffset = boxArea.m_offset;
			cardSlot.m_emptyAreaSize = boxArea.m_size;
		}
	}
}
