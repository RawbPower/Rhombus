#include "rbpch.h"
#include "CardSlotComponent.h"

namespace rhombus
{
	void CardSlotComponent::AddCard(Entity card) 
	{ 
		m_cardStack.push_back(card); 
		m_isOccupied = true;
		ArrangeCardPositions();
	}

	void CardSlotComponent::ArrangeCardPositions()
	{
		const int numOfCards = m_cardStack.size();
		int i = 0;
		for (Entity card : m_cardStack)
		{
			TransformComponent& transform = card.GetComponent<TransformComponent>();

			transform.SetPositionByLayerSection(Z_LAYER::FOREGROUND_1_LAYER, i, numOfCards);
			i++;
		}
	}
}