#pragma once

#include "Rhombus.h"

#include <list>

class CardSlotComponent : public ComponentBase
{
public:
	enum SlotType {SLOT_TYPE_SINGLE = 0, SLOT_TYPE_STACK, SLOT_TYPE_STAGGERED, SLOT_TYPE_COUNT};

	CardSlotComponent() = default;
	CardSlotComponent(const CardSlotComponent&) = default;

	virtual void OnComponentAdded() override;

	int GetIsOccupied() const { return m_isOccupied; }
	void SetIsOccupied(bool occupied) { m_isOccupied = occupied; }

	bool CanAcceptCards();
	void AddCard(Entity card);
	void RemoveCard(Entity card);

	void UpdateCardStack();

	void SetSlotType(int slotType)
	{
		m_slotType = (SlotType)slotType;
	}

	Vec2& GetStaggeredOffset() { return m_staggeredOffset; }
	void SetStaggeredOffset(Vec2 offset) { m_staggeredOffset = offset; }

	const SlotType GetSlotType() const{ return m_slotType; }
	SlotType& GetSlotTypeNonConst() { return m_slotType; }

	const char* GetSlotTypeName()
	{
		return GetSlotTypeName(m_slotType);
	}

	static const char* GetSlotTypeName(int slotType)
	{
		if (slotType >= 0 && slotType < SLOT_TYPE_COUNT)
		{
			sm_slotTypeNameList[slotType];
		}
		else
		{
			return "Invalid";
		}
	}

	static const char** GetSlotTypeNameList()
	{
		return sm_slotTypeNameList;
	}

public:
	std::list<Entity> m_cardStack;
	Vec2 m_staggeredOffset = Vec2(0.0f, -16.0f);

private:
	SlotType m_slotType = SLOT_TYPE_SINGLE;
	bool m_isOccupied = false;

	Vec2 m_emptyAreaOffset = { 0.0f, 0.0f };
	Vec2 m_emptyAreaSize = { 0.5f, 0.5f };

	static const char* sm_slotTypeNameList[SLOT_TYPE_COUNT];
};
