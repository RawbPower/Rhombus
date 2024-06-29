#pragma once

#include "Rhombus.h"

class CardComponent : public ComponentBase
{
public:

	enum Suit { SUIT_HEART = 0, SUIT_DIAMOND, SUIT_SPADE, SUIT_CLUB, SUIT_COUNT };

	CardComponent() = default;
	CardComponent(const CardComponent&) = default;

	int GetColor() const { return m_rank; }
	void SetColor(int rank) { m_rank = rank; }

	Suit GetSuit() const { return m_suit; }
	void SetSuit(Suit suit) { m_suit = suit; }

	int GetIsHeld() const { return m_isHeld; }
	void SetIsHeld(bool held) { m_isHeld = held; }

	int GetIsAvailable() const { return m_isAvailable; }
	void SetIsAvailable(bool available) { m_isAvailable = available; }

	Vec2 GetPreviousPosition() const { return m_previousPosition; }
	void SetPreviousPosition(Vec2 prevPos) { m_previousPosition = prevPos; }

	Entity GetCurrentSlot() const { return m_currentSlot; }
	void SetCurrentlSlot(Entity cardSlot) { m_currentSlot = cardSlot; }
	void ResetCurrentlSlot(Vec2 prevPos) { m_currentSlot = Entity(); }

	const char* GetSlotTypeName()
	{
		return GetSlotTypeName(m_suit);
	}

	const char* GetSlotTypeName(int suit)
	{
		if (suit >= 0 && suit < SUIT_COUNT)
		{
			m_suitNameList[suit];
		}
		else
		{
			return "Invalid";
		}
	}

	const char** GetSlotTypeNameList()
	{
		return m_suitNameList;
	}

public:
	int m_rank;
	Suit m_suit;

private:
	bool m_isHeld = false;
	bool m_isAvailable = true;
	Vec2 m_previousPosition = Vec2(0.0f);
	Entity m_currentSlot;

	const char* m_suitNameList[SUIT_COUNT] = { "Heart", "Diamond", "Spade", "Club"};
};
