#include "CardSlotComponent.h"

#include "CardComponent.h"

#include "Rhombus/Scripting/ScriptEngine.h"

const char* CardSlotComponent::sm_slotLayoutNameList[SLOT_LAYOUT_COUNT] = { "Single", "Stack", "Staggered" };
const char* CardSlotComponent::sm_slotTypeNameList[SLOT_TYPE_COUNT] = { "Column", "Site", "Freecell", "Stock", "Wastepile", "Monster"};

const char* CardSlotComponent::sm_revelationNameList[REVELATION_COUNT] = { "Open", "Closed" };
const char* CardSlotComponent::sm_rankOrderingNameList[RANK_ORDERING_COUNT] = { "Any", "Ascending", "Descending" };
const char* CardSlotComponent::sm_suitOrderingNameList[SUIT_ORDERING_COUNT] = { "Any", "DifferentSuit", "DifferentColor", "SameSuit" };
const char* CardSlotComponent::sm_emptyColumnTypeNameList[EMPTY_COLUMN_COUNT] = { "Any", "King" };

void CardSlotComponent::OnComponentAdded()
{
}

void CardSlotComponent::UpdateAllowedCards()
{
	const CardComponent* topCardData = m_cardStack.size() > 0 ? &m_cardStack.back().GetComponentRead<CardComponent>() : nullptr;
	switch (m_slotType)
	{
	case SLOT_TYPE_COLUMN:
		if (topCardData)
		{
			uint32_t allowedRanks, allowedSuits;
			SuitOrdering suitOrdering = (topCardData->m_packingSuitOrderOverride >= 0 && topCardData->m_packingSuitOrderOverride != SUIT_ORDERING_COUNT) ? (SuitOrdering)topCardData->m_packingSuitOrderOverride : sm_cardSlotData.packingSuitOrder;
			GetAllowedNextCardsInSequence(*topCardData, sm_cardSlotData.packingRankOrder, suitOrdering, sm_cardSlotData.canLoop, sm_cardSlotData.loopMax, m_allowedRanks, m_allowedSuits);
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
		if (!topCardData)
		{
			if (m_siteInfo.m_foundationRank < 0)
			{
				m_allowedRanks = -1;
			}
			else
			{
				m_allowedRanks = 1ul << m_siteInfo.m_foundationRank;
			}

			if ((CardComponent::Suit)m_siteInfo.m_foundationSuit == CardComponent::SUIT_WILD)
			{
				m_allowedSuits = -1;
			}
			else
			{
				m_allowedSuits = 1ul << (uint32_t)m_siteInfo.m_foundationSuit;
			}
		}
		else
		{
			GetAllowedNextCardsInSequence(*topCardData, m_siteInfo.m_rankOrdering, m_siteInfo.m_suitOrdering, m_siteInfo.m_canLoop, m_siteInfo.m_loopMax, m_allowedRanks, m_allowedSuits);
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

	if (m_slotType == SLOT_TYPE_COLUMN)
	{
		int sequenceSize = 1;
		const CardComponent* lowerCard = m_cardStack.size() > 0 ? &m_cardStack.back().GetComponentRead<CardComponent>() : nullptr;
		const CardComponent* higherCard = nullptr;

		std::list<Entity>::reverse_iterator it;

		bool skippedFirstCard = false;
		if (lowerCard)
		{
			for (it = m_cardStack.rbegin(); it != m_cardStack.rend(); it++)
			{
				if (!skippedFirstCard)
				{
					skippedFirstCard = true;
					continue;
				}

				higherCard = &it->GetComponentRead<CardComponent>();
				if (higherCard->m_type == CardComponent::TYPE_MONSTER)
				{
					break;
				}

				uint32_t allowedRanks, allowedSuits;
				SuitOrdering suitOrdering = (higherCard->m_packingSuitOrderOverride >= 0 && higherCard->m_packingSuitOrderOverride != SUIT_ORDERING_COUNT) ? (SuitOrdering)higherCard->m_packingSuitOrderOverride : sm_cardSlotData.packingSuitOrder;
				GetAllowedNextCardsInSequence(*higherCard, sm_cardSlotData.packingRankOrder, suitOrdering, sm_cardSlotData.canLoop, sm_cardSlotData.loopMax, allowedRanks, allowedSuits);

				if (IsCardAllowedInSlot(lowerCard->m_rank, lowerCard->m_suit, allowedRanks, allowedSuits))
				{
					sequenceSize++;
					lowerCard = higherCard;
					higherCard = nullptr;
				}
				else
				{
					break;
				}
			}
		}

		m_sequenceLength = sequenceSize;
	}
}

void CardSlotComponent::GetAllowedNextCardsInSequence(const CardComponent& card, RankOrdering rankOrdering, SuitOrdering suitOrdering, bool canLoop, int loopMax, uint32_t& allowedRanks, uint32_t& allowedSuits) const
{
	if (card.m_rank == 0)		// Wild Card
	{
		allowedRanks = -1;
		allowedSuits = -1;
	}
	else
	{
		switch (rankOrdering)
		{
		case RANK_ORDERING_ANY:
			allowedRanks = -1;
			break;
		case RANK_ORDERING_ASCENDING:
			allowedRanks = (1ul << (card.m_rank + 1)) | 1;
			if (canLoop && card.m_rank == loopMax)
			{
				allowedRanks = (1ul << 1) | 1;
			}
			break;
		case RANK_ORDERING_DESCENDING:
			allowedRanks = (card.m_rank > 0 ? 1ul << (card.m_rank - 1) : 0) | 1;
			if (canLoop && card.m_rank == 1)
			{
				allowedRanks = (1ul << loopMax) | 1;
			}
			break;
		default:
			break;
		}

		switch (suitOrdering)
		{
		case SUIT_ORDERING_ANY:
			allowedSuits = -1;
			break;
		case SUIT_ORDERING_DIFFERENT_SUIT:
			allowedSuits = ~(1ul << (uint32_t)card.m_suit);
			break;
		case SUIT_ORDERING_DIFFERENT_COLOR:
			if (card.m_suit == CardComponent::Suit::SUIT_HEART || card.m_suit == CardComponent::Suit::SUIT_DIAMOND)
			{
				allowedSuits = (1ul << (uint32_t)CardComponent::Suit::SUIT_SPADE) | (1ul << (uint32_t)CardComponent::Suit::SUIT_CLUB);
			}
			else if (card.m_suit == CardComponent::Suit::SUIT_SPADE || card.m_suit == CardComponent::Suit::SUIT_CLUB)
			{
				allowedSuits = (1ul << (uint32_t)CardComponent::Suit::SUIT_HEART) | (1ul << (uint32_t)CardComponent::Suit::SUIT_DIAMOND);
			}
			else
			{
				allowedSuits = -1;
			}
			break;
		case SUIT_ORDERING_SAME_SUIT:
			allowedSuits = (1ul << (uint32_t)card.m_suit) | (1ul << (uint32_t)CardComponent::Suit::SUIT_WILD);
			break;
		default:
			break;
		}
	}
}

bool CardSlotComponent::IsCardAllowedInSlot(int rank, CardComponent::Suit suit) const
{
	const bool isRankAllowed = (m_allowedRanks & (1ul << rank)) != 0;
	const bool isSuitAllowed = (m_allowedSuits & (1ul << (uint32_t)suit)) != 0;
	return isSuitAllowed && isRankAllowed;
}

bool CardSlotComponent::IsCardAllowedInSlot(int rank, CardComponent::Suit suit, uint32_t allowedRanks, uint32_t allowedSuits) const
{
	const bool isRankAllowed = (allowedRanks & (1ul << rank)) != 0;
	const bool isSuitAllowed = (allowedSuits & (1ul << (uint32_t)suit)) != 0;
	return isSuitAllowed && isRankAllowed;
}

bool CardSlotComponent::CanAcceptCards() const
{
	return m_slotLayout != SLOT_LAYOUT_SINGLE || m_cardStack.size() == 0;
}

bool CardSlotComponent::CanAcceptSequences() const
{
	return m_slotType == SLOT_TYPE_COLUMN;
}

void CardSlotComponent::AddCard(Entity card) 
{ 
	CardComponent& cardComponent = card.GetComponent<CardComponent>();
	m_cardStack.push_back(card); 
	m_isOccupied = true;
	UpdateAllowedCards();
	UpdateCardStack();
}

void CardSlotComponent::RemoveCard(Entity card)
{ 
	m_cardStack.remove(card); 
	UpdateAllowedCards();
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

		if (cardComponent.GetIsHeld())
		{
			continue;
		}

		if (m_slotLayout == SLOT_LAYOUT_STAGGERED)
		{
			transform.m_position.x = slotTransform.m_position.x + i * m_staggeredOffset.x;
			transform.m_position.y = slotTransform.m_position.y + i * m_staggeredOffset.y;
		}
		transform.SetPositionByLayerSection(Z_LAYER::FOREGROUND_1_LAYER, i, numOfCards);
		cardComponent.SetIsAvailable(i >= (numOfCards - m_sequenceLength));
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

void CardSlotComponent::GetCardSequence(Entity topCard, std::queue<Entity>& cardSequenceOut) const
{
	int cardsLeftInSlot = m_cardStack.size();
	bool foundSequence = false;
	std::list<Entity>::const_iterator it;
	for (it = m_cardStack.begin(); it != m_cardStack.end(); ++it) 
	{
		Entity card = *it;
		if (foundSequence)
		{
			cardSequenceOut.push(card);
		}
		else
		{
			if (card == topCard)
			{
				cardSequenceOut.push(card);
				if (cardsLeftInSlot <= m_sequenceLength)
				{
					foundSequence = true;
				}
				else
				{
					break;
				}
			}
		}
		cardsLeftInSlot--;
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

	ScriptEngine::GetField("PackingRankOrder");
	sm_cardSlotData.packingRankOrder = (RankOrdering)ScriptEngine::GetEnumFromName(ScriptEngine::GetString(), sm_rankOrderingNameList, RankOrdering::RANK_ORDERING_COUNT);
	ScriptEngine::Pop();

	ScriptEngine::GetField("PackingSuitOrder");
	sm_cardSlotData.packingSuitOrder = (SuitOrdering)ScriptEngine::GetEnumFromName(ScriptEngine::GetString(), sm_suitOrderingNameList, SuitOrdering::SUIT_ORDERING_COUNT);
	ScriptEngine::Pop();

	ScriptEngine::GetField("EmptyColumnType");
	sm_cardSlotData.emptyColumnType = (EmptyColumnType)ScriptEngine::GetEnumFromName(ScriptEngine::GetString(), sm_emptyColumnTypeNameList, EmptyColumnType::EMPTY_COLUMN_COUNT);
	ScriptEngine::Pop();
}