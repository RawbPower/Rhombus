#pragma once

#include "Rhombus.h"

#include "CardComponent.h"

#include <list>
#include <bitset>

class CardSlotComponent : public ComponentBase
{
public:
	enum SlotLayout {SLOT_LAYOUT_SINGLE = 0, SLOT_LAYOUT_STACK, SLOT_LAYOUT_STAGGERED, SLOT_LAYOUT_COUNT};
	enum SlotType {SLOT_TYPE_COLUMN = 0, SLOT_TYPE_SITE, SLOT_TYPE_FREECELL, SLOT_TYPE_STOCK, SLOT_TYPE_WASTEPILE, SLOT_TYPE_MONSTER, SLOT_TYPE_COUNT};

	enum Revelation { REVELATION_OPEN = 0, REVELATION_CLOSED, REVELATION_COUNT };
	enum RankOrdering { RANK_ORDERING_ANY = 0, RANK_ORDERING_ASCENDING, RANK_ORDERING_DESCENDING, RANK_ORDERING_COUNT };
	enum SuitOrdering { SUIT_ORDERING_ANY = 0, SUIT_ORDERING_DIFFERENT_SUIT, SUIT_ORDERING_DIFFERENT_COLOR, SUIT_ORDERING_SAME_SUIT, SUIT_ORDERING_COUNT };
	enum EmptyColumnType { EMPTY_COLUMN_ANY = 0, EMPTY_COLUMN_KING, EMPTY_COLUMN_COUNT };

	struct CardSlotData
	{
		int columns = 0;
		int sites = 0;
		int freecells = 0;
		int stocks = 0;
		int wastepiles = 0;

		Revelation revelation = REVELATION_OPEN;

		RankOrdering packingRankOrder = RANK_ORDERING_ANY;
		SuitOrdering packingSuitOrder = SUIT_ORDERING_ANY;
		bool canLoop = false;
		int loopMax = 13;
		EmptyColumnType emptyColumnType = EMPTY_COLUMN_ANY;

		EntityID mainDiscard = INVALID_ENTITY;
		EntityID monsterDiscard = INVALID_ENTITY;
	};

	struct SiteInfo
	{
		uint32_t m_foundationSuit = 0;
		int m_foundationRank = -1;
		RankOrdering m_rankOrdering = RANK_ORDERING_ANY;
		SuitOrdering m_suitOrdering = SUIT_ORDERING_ANY;
		bool m_canLoop = false;
		int m_loopMax = 13;
	};

	CardSlotComponent() = default;
	CardSlotComponent(const CardSlotComponent&) = default;

	virtual void OnComponentAdded() override;

	void UpdateAllowedCards();
	void GetAllowedNextCardsInSequence(const CardComponent& card, RankOrdering rankOrdering, SuitOrdering suitOrdering, bool canLoop, int loopMax, uint32_t& allowedRanks, uint32_t& allowedSuits) const;
	bool IsCardAllowedInSlot(int rank, CardComponent::Suit suit) const;
	bool IsCardAllowedInSlot(int rank, CardComponent::Suit suit, uint32_t allowedRanks, uint32_t allowedSuits) const;

	int GetIsOccupied() const { return m_isOccupied; }
	void SetIsOccupied(bool occupied) { m_isOccupied = occupied; }

	bool CanAcceptCards() const;
	bool CanAcceptSequences() const;
	void AddCard(Entity card);
	void RemoveCard(Entity card);

	void UpdateCardStack();

	void GetCardSequence(Entity topCard, std::queue<Entity>& cardSequenceOut) const;

	Vec2& GetStaggeredOffset() { return m_staggeredOffset; }
	void SetStaggeredOffset(Vec2 offset) { m_staggeredOffset = offset; }

	uint32_t GetSequenceLength() const { return m_sequenceLength; }

	Entity GetMonsterSlot() const { return m_monsterSlot; }

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

	const char* GetSlotTypeName() const
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

	static void InitCardSlotData(Scene& scene);

public:
	std::list<Entity> m_cardStack;
	Vec2 m_staggeredOffset = Vec2(0.0f, -16.0f);
	UUID m_monsterBattleSite = 0;

	SiteInfo m_siteInfo;

	Vec2 m_emptyAreaOffset = { 0.0f, 0.0f };
	Vec2 m_emptyAreaSize = { 0.5f, 0.5f };

private:
	friend class CardSlotSystem;

	SlotLayout m_slotLayout = SLOT_LAYOUT_SINGLE;
	SlotType m_slotType = SLOT_TYPE_COLUMN;
	bool m_isOccupied = false;
	Entity m_monsterSlot;

	uint32_t m_allowedRanks;
	uint32_t m_allowedSuits;
	int m_sequenceLength = 1;

public:
	inline static CardSlotData sm_cardSlotData;

	static const char* sm_revelationNameList[REVELATION_COUNT];
	static const char* sm_rankOrderingNameList[RANK_ORDERING_COUNT];
	static const char* sm_suitOrderingNameList[SUIT_ORDERING_COUNT];
	static const char* sm_emptyColumnTypeNameList[EMPTY_COLUMN_COUNT];
	static const char* sm_slotLayoutNameList[SLOT_LAYOUT_COUNT];
	static const char* sm_slotTypeNameList[SLOT_TYPE_COUNT];
};
