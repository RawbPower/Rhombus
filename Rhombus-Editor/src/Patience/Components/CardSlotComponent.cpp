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

void CardSlotComponent::UpdateAllowCards()
{

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