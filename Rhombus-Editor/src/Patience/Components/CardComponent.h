#pragma once

#include "Rhombus.h"

class CardComponent : public ComponentBase
{
public:

	enum Suit { SUIT_HEART = 0, SUIT_DIAMOND, SUIT_SPADE, SUIT_CLUB, SUIT_TRUMP, SUIT_WILD, SUIT_COUNT };
	enum Type { TYPE_REGULAR = 0, TYPE_MONSTER, TYPE_COUNT };

	struct MonsterStats
	{
		int m_health;
	};

	CardComponent() = default;
	CardComponent(const CardComponent&) = default;

	int GetColor() const { return m_rank; }
	void SetColor(int rank) { m_rank = rank; }

	Suit GetSuit() const { return m_suit; }
	void SetSuit(Suit suit) { m_suit = suit; }

	bool GetIsHeld() const { return m_isHeld; }
	void SetIsHeld(bool held) { m_isHeld = held; }

	Vec2 GetHeldOffset() const { return m_heldOffset; }
	void SetHeldOffset(Vec2 heldOffset) { m_heldOffset = heldOffset; }

	int GetIsHovered() const { return m_isHovered; }
	void SetIsHovered(bool hovered) { m_isHovered = hovered; }

	int GetIsAvailable() const { return m_isAvailable; }
	void SetIsAvailable(bool available) { m_isAvailable = available; }

	Vec3 GetPreviousPosition() const { return m_previousPosition; }
	void SetPreviousPosition(Vec3 prevPos) { m_previousPosition = prevPos; }

	Entity GetCurrentSlot() const { return m_currentSlot; }
	void SetCurrentlSlot(Entity cardSlot) { m_currentSlot = cardSlot; }
	void ResetCurrentlSlot() { m_currentSlot = Entity(); }

	bool CanBeHeld() const { return m_type != TYPE_MONSTER; }

	const char* GetSuitName()
	{
		return GetSuitName(m_suit);
	}

	static const char* GetSuitName(int suit)
	{
		if (suit >= 0 && suit < SUIT_COUNT)
		{
			sm_suitNameList[suit];
		}
		else
		{
			return "Invalid";
		}
	}

	static const char** GetSuitNameList()
	{
		return sm_suitNameList;
	}

	static const char** GetTypeNameList()
	{
		return sm_typeNameList;
	}

public:
	int m_rank;
	Suit m_suit;
	int m_packingTypeOverride = -1;
	Type m_type = TYPE_REGULAR;
	MonsterStats m_monsterStats;		// Late initialize this

private:
	bool m_isHeld = false;
	bool m_isHovered = false;
	bool m_isAvailable = true;
	Vec2 m_heldOffset = Vec3(0.0f);
	Vec3 m_previousPosition = Vec3(0.0f);
	Entity m_currentSlot;

	static const char* sm_suitNameList[SUIT_COUNT];
	static const char* sm_typeNameList[TYPE_COUNT];
};
