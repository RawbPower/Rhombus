#include "CardSlotComponent.h"

#include "CardComponent.h"

const char* CardSlotComponent::sm_slotTypeNameList[SLOT_TYPE_COUNT] = { "Single", "Stack", "Staggered" };

void CardSlotComponent::OnComponentAdded()
{
	const BoxArea2DComponent& area = GetOwnerEntity().GetComponentRead<BoxArea2DComponent>();
	m_emptyAreaOffset = area.m_offset;
	m_emptyAreaSize = area.m_size;
}

bool CardSlotComponent::CanAcceptCards()
{
	return m_slotType != SLOT_TYPE_SINGLE || m_cardStack.size() == 0;
}

void CardSlotComponent::AddCard(Entity card) 
{ 
	m_cardStack.push_back(card); 
	m_isOccupied = true;
	UpdateCardStack();
}

void CardSlotComponent::RemoveCard(Entity card)
{ 
	m_cardStack.remove(card); 
	UpdateCardStack();
}

void CardSlotComponent::UpdateCardStack()
{
	const int numOfCards = m_cardStack.size();
	int i = 0;
	for (Entity card : m_cardStack)
	{
		TransformComponent& transform = card.GetComponent<TransformComponent>();
		const TransformComponent& slotTransform = GetOwnerEntity().GetComponentRead<TransformComponent>();
		CardComponent& cardComponent = card.GetComponent<CardComponent>();

		if (m_slotType == SLOT_TYPE_STAGGERED)
		{
			transform.m_position.x = slotTransform.m_position.x + i * m_staggeredOffset.x;
			transform.m_position.y = slotTransform.m_position.y + i * m_staggeredOffset.y;
		}
		transform.SetPositionByLayerSection(Z_LAYER::FOREGROUND_1_LAYER, i, numOfCards);
		cardComponent.SetIsAvailable(i == (numOfCards - 1));
		i++;
	}

	if (m_slotType == SLOT_TYPE_STAGGERED)
	{
		float sizeMult = numOfCards > 0 ? ((float)numOfCards - 1.0f) * 0.5f : 0.0f;
		BoxArea2DComponent& area = GetOwnerEntity().GetComponent<BoxArea2DComponent>();
		area.m_offset = m_emptyAreaOffset + m_staggeredOffset * sizeMult;
		area.m_size = m_emptyAreaSize + Vec2::Abs(m_staggeredOffset) * sizeMult;
	}
}