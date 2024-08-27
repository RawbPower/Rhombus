#include "CardSlotComponent.h"

#include "CardComponent.h"

#include "Rhombus/Scripting/ScriptEngine.h"

const char* CardSlotComponent::sm_slotLayoutNameList[SLOT_LAYOUT_COUNT] = { "Single", "Stack", "Staggered" };
const char* CardSlotComponent::sm_slotTypeNameList[SLOT_TYPE_COUNT] = { "Column", "Site", "Freecell", "Stock", "Wastepile" };

const char* CardSlotComponent::sm_revelationNameList[REVELATION_COUNT] = { "Open", "Closed" };
const char* CardSlotComponent::sm_orderingNameList[ORDERING_COUNT] = { "Ascending", "Descending" };
const char* CardSlotComponent::sm_packingTypeNameList[PACKING_COUNT] = { "Any", "DifferentSuit", "DifferentColor", "SameSuit" };
const char* CardSlotComponent::sm_emptyColumnTypeNameList[EMPTY_COLUMN_COUNT] = { "Any", "King" };

void CardSlotComponent::OnComponentAdded()
{
	const BoxArea2DComponent& area = GetOwnerEntity().GetComponentRead<BoxArea2DComponent>();
	m_emptyAreaOffset = area.m_offset;
	m_emptyAreaSize = area.m_size;
}

void CardSlotComponent::UpdateAllowedCards()
{
	CardComponent* topCardData = m_cardStack.size() > 0 ? &m_cardStack.back().GetComponent<CardComponent>() : nullptr;
	switch (m_slotType)
	{
	case SLOT_TYPE_COLUMN:
		if (topCardData)
		{
			if (topCardData->m_rank == 0)		// Wild Card
			{
				m_allowedRanks = -1;
				m_allowedSuits = -1;
			}
			else
			{
				switch (sm_cardSlotData.packingOrder)
				{
				case ORDERING_ASCENDING:
					m_allowedRanks = (1ul << (topCardData->m_rank + 1)) | 1;
					break;
				case ORDERING_DESCENDING:
					m_allowedRanks = (topCardData->m_rank > 0 ? 1ul << (topCardData->m_rank - 1) : 0) | 1;
					break;
				default:
					break;
				}	

				PackingType packingType = (topCardData->m_packingTypeOverride >= 0 && topCardData->m_packingTypeOverride != PACKING_COUNT) ? (PackingType)topCardData->m_packingTypeOverride : sm_cardSlotData.packingType;
				switch (packingType)
				{
				case PACKING_ANY:
					m_allowedSuits = -1;
					break;
				case PACKING_DIFFERENT_SUIT:
					m_allowedSuits = ~(1ul << (uint32_t)topCardData->m_suit);
					break;
				case PACKING_DIFFERENT_COLOR:
					if (topCardData->m_suit == CardComponent::Suit::SUIT_HEART || topCardData->m_suit == CardComponent::Suit::SUIT_DIAMOND)
					{
						m_allowedSuits = (1ul << (uint32_t)CardComponent::Suit::SUIT_SPADE) | (1ul << (uint32_t)CardComponent::Suit::SUIT_CLUB);
					}
					else if (topCardData->m_suit == CardComponent::Suit::SUIT_SPADE || topCardData->m_suit == CardComponent::Suit::SUIT_CLUB)
					{
						m_allowedSuits = (1ul << (uint32_t)CardComponent::Suit::SUIT_HEART) | (1ul << (uint32_t)CardComponent::Suit::SUIT_DIAMOND);
					}
					else
					{
						m_allowedSuits = -1;
					}
					break;
				case PACKING_SAME_SUIT:
					m_allowedSuits = (1ul << (uint32_t)topCardData->m_suit) | (1ul << (uint32_t)CardComponent::Suit::SUIT_WILD);
					break;
				default:
					break;
				}
			}
		}
		else
		{
			switch (sm_cardSlotData.emptyColumnType)
			{
			case EMPTY_COLUMN_KING:
				m_allowedRanks = (1ul << 13) | (1ul << 0);
				break;
			default:
				m_allowedRanks = -1;
				break;
			}
			m_allowedSuits = -1;
		}

		break;
	case SLOT_TYPE_SITE:
		m_allowedSuits = 1ul << (uint32_t)m_suitFoundation;
		if (!topCardData)
		{
			if ((CardComponent::Suit)m_suitFoundation != CardComponent::SUIT_TRUMP)
			{
				m_allowedRanks = 1ul << sm_cardSlotData.foundationRank;
			}
			else
			{
				m_allowedRanks = 1ul << 21;
			}
		}
		else
		{
			CardSlotComponent::Ordering ordering = (CardComponent::Suit)m_suitFoundation != CardComponent::SUIT_TRUMP ? sm_cardSlotData.buildingOrder : ORDERING_DESCENDING;
			switch (ordering)
			{
			case ORDERING_ASCENDING:
				m_allowedRanks = 1ul << (topCardData->m_rank + 1);
				break;
			case ORDERING_DESCENDING:
				m_allowedRanks = topCardData->m_rank > 0 ? 1ul << (topCardData->m_rank - 1) : 0;
				break;
			default:
				break;
			}
		}
		break;
	case SLOT_TYPE_FREECELL:
		m_allowedRanks = -1;
		m_allowedSuits = -1;
		break;
	default:
		m_allowedRanks = -1;
		m_allowedSuits = -1;
		break;
	}
}

bool CardSlotComponent::IsCardAllowedInSlot(int rank, CardComponent::Suit suit)
{
	const bool isRankAllowed = (m_allowedRanks & (1ul << rank)) != 0;
	const bool isSuitAllowed = (m_allowedSuits & (1ul << (uint32_t)suit)) != 0;
	return isSuitAllowed && isRankAllowed;
}


bool CardSlotComponent::CanAcceptCards()
{
	return m_slotLayout != SLOT_LAYOUT_SINGLE || m_cardStack.size() == 0;
}

void CardSlotComponent::AddCard(Entity card) 
{ 
	m_cardStack.push_back(card); 
	m_isOccupied = true;
	UpdateCardStack();
	UpdateAllowedCards();
}

void CardSlotComponent::RemoveCard(Entity card)
{ 
	m_cardStack.remove(card); 
	UpdateCardStack();
	UpdateAllowedCards();
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

		if (m_slotLayout == SLOT_LAYOUT_STAGGERED)
		{
			transform.m_position.x = slotTransform.m_position.x + i * m_staggeredOffset.x;
			transform.m_position.y = slotTransform.m_position.y + i * m_staggeredOffset.y;
		}
		transform.SetPositionByLayerSection(Z_LAYER::FOREGROUND_1_LAYER, i, numOfCards);
		cardComponent.SetIsAvailable(i == (numOfCards - 1));
		i++;
	}

	if (m_slotLayout == SLOT_LAYOUT_STAGGERED)
	{
		float sizeMult = numOfCards > 0 ? ((float)numOfCards - 1.0f) * 0.5f : 0.0f;
		BoxArea2DComponent& area = GetOwnerEntity().GetComponent<BoxArea2DComponent>();
		area.m_offset = m_emptyAreaOffset + m_staggeredOffset * sizeMult;
		area.m_size = m_emptyAreaSize + Vec2::Abs(m_staggeredOffset) * sizeMult;
	}
}

void CardSlotComponent::InitCardSlotData()
{
	ScriptEngine::GetField("Columns");
	sm_cardSlotData.columns = ScriptEngine::GetInt();
	ScriptEngine::Pop();

	ScriptEngine::GetField("Sites");
	sm_cardSlotData.sites = ScriptEngine::GetInt();
	ScriptEngine::Pop();

	ScriptEngine::GetField("Freecells");
	sm_cardSlotData.freecells = ScriptEngine::GetInt();
	ScriptEngine::Pop();

	ScriptEngine::GetField("Stocks");
	sm_cardSlotData.stocks = ScriptEngine::GetInt();
	ScriptEngine::Pop();

	ScriptEngine::GetField("Wastepiles");
	sm_cardSlotData.wastepiles = ScriptEngine::GetInt();
	ScriptEngine::Pop();

	ScriptEngine::GetField("Revelation");
	sm_cardSlotData.revelation = (Revelation)ScriptEngine::GetEnumFromName(ScriptEngine::GetString(), sm_revelationNameList, Revelation::REVELATION_COUNT);
	ScriptEngine::Pop();

	ScriptEngine::GetField("BuildingOrder");
	sm_cardSlotData.buildingOrder = (Ordering)ScriptEngine::GetEnumFromName(ScriptEngine::GetString(), sm_orderingNameList, Ordering::ORDERING_COUNT);
	ScriptEngine::Pop();

	ScriptEngine::GetField("FoundationRank");
	sm_cardSlotData.foundationRank = ScriptEngine::GetInt();
	ScriptEngine::Pop();

	ScriptEngine::GetField("PackingOrder");
	sm_cardSlotData.packingOrder = (Ordering)ScriptEngine::GetEnumFromName(ScriptEngine::GetString(), sm_orderingNameList, Ordering::ORDERING_COUNT);
	ScriptEngine::Pop();

	ScriptEngine::GetField("PackingType");
	sm_cardSlotData.packingType = (PackingType)ScriptEngine::GetEnumFromName(ScriptEngine::GetString(), sm_packingTypeNameList, PackingType::PACKING_COUNT);
	ScriptEngine::Pop();

	ScriptEngine::GetField("EmptyColumnType");
	sm_cardSlotData.emptyColumnType = (EmptyColumnType)ScriptEngine::GetEnumFromName(ScriptEngine::GetString(), sm_emptyColumnTypeNameList, EmptyColumnType::EMPTY_COLUMN_COUNT);
	ScriptEngine::Pop();
}