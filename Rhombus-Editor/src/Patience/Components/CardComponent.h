#pragma once

#include "Rhombus.h"

class CardComponent : public ComponentBase
{
public:

	enum Suit { SUIT_HEART = 0, SUIT_DIAMOND, SUIT_SPADE, SUIT_CLUB, SUIT_TRUMP, SUIT_WILD, SUIT_COUNT };

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

	Vec3 GetPreviousPosition() const { return m_previousPosition; }
	void SetPreviousPosition(Vec3 prevPos) { m_previousPosition = prevPos; }

	Entity GetCurrentSlot() const { return m_currentSlot; }
	void SetCurrentlSlot(Entity cardSlot) { m_currentSlot = cardSlot; }
	void ResetCurrentlSlot() { m_currentSlot = Entity(); }

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

public:
	int m_rank;
	Suit m_suit;
	int m_packingTypeOverride;

private:
	bool m_isHeld = false;
	bool m_isAvailable = true;
	Vec3 m_previousPosition = Vec3(0.0f);
	Entity m_currentSlot;

	static const char* sm_suitNameList[SUIT_COUNT];
};
