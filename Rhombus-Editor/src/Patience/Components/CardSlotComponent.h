#pragma once

#include "Rhombus.h"

#include <list>

class CardSlotComponent : public ComponentBase
{
public:
	enum SlotLayout {SLOT_LAYOUT_SINGLE = 0, SLOT_LAYOUT_STACK, SLOT_LAYOUT_STAGGERED, SLOT_LAYOUT_COUNT};
	enum SlotType {SLOT_TYPE_COLUMN = 0, SLOT_TYPE_SITE, SLOT_TYPE_FREECELL, SLOT_TYPE_STOCK, SLOT_TYPE_WASTEPILE, SLOT_TYPE_COUNT};

	CardSlotComponent() = default;
	CardSlotComponent(const CardSlotComponent&) = default;

	virtual void OnComponentAdded() override;

	int GetIsOccupied() const { return m_isOccupied; }
	void SetIsOccupied(bool occupied) { m_isOccupied = occupied; }

	bool CanAcceptCards();
	void AddCard(Entity card);
	void RemoveCard(Entity card);

	void UpdateCardStack();

	Vec2& GetStaggeredOffset() { return m_staggeredOffset; }
	void SetStaggeredOffset(Vec2 offset) { m_staggeredOffset = offset; }

	// Slot Layout
	const SlotLayout GetSlotLayout() const{ return m_slotLayout; }
	SlotLayout& GetSlotLayoutNonConst() { return m_slotLayout; }
	void SetSlotLayout(int slotLayout) { m_slotLayout = (SlotLayout)slotLayout; }

	const char* GetSlotLayoutName()
	{
		return GetSlotLayoutName(m_slotLayout);
	}

	static const char* GetSlotLayoutName(int slotLayout)
	{
		if (slotLayout >= 0 && slotLayout < SLOT_LAYOUT_COUNT)
		{
			sm_slotLayoutNameList[slotLayout];
		}
		else
		{
			return "Invalid";
		}
	}

	static const char** GetSlotLayoutNameList()
	{
		return sm_slotLayoutNameList;
	}

	// Slot Type
	const SlotType GetSlotType() const { return m_slotType; }
	SlotType& GetSlotTypeNonConst() { return m_slotType; }
	void SetSlotType(int slotType) { m_slotType = (SlotType)slotType; }

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
	SlotLayout m_slotLayout = SLOT_LAYOUT_SINGLE;
	SlotType m_slotType = SLOT_TYPE_COLUMN;
	bool m_isOccupied = false;

	Vec2 m_emptyAreaOffset = { 0.0f, 0.0f };
	Vec2 m_emptyAreaSize = { 0.5f, 0.5f };

	static const char* sm_slotLayoutNameList[SLOT_LAYOUT_COUNT];
	static const char* sm_slotTypeNameList[SLOT_TYPE_COUNT];
};
