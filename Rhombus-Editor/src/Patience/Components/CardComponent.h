#pragma once

#include "Rhombus.h"

namespace rhombus
{
	class CardComponent : public ComponentBase
	{
	public:
		CardComponent() = default;
		CardComponent(const CardComponent&) = default;

		int GetColor() const { return m_rank; }
		void SetColor(int rank) { m_rank = rank; }

		int GetSuit() const { return m_suit; }
		void SetSuit(int suit) { m_suit = suit; }

		int GetIsHeld() const { return m_isHeld; }
		void SetIsHeld(bool held) { m_isHeld = held; }

		int GetIsAvailable() const { return m_isAvailable; }
		void SetIsAvailable(bool available) { m_isAvailable = available; }

		Vec2 GetPreviousPosition() const { return m_previousPosition; }
		void SetPreviousPosition(Vec2 prevPos) { m_previousPosition = prevPos; }

		Entity GetCurrentSlot() const { return m_currentSlot; }
		void SetCurrentlSlot(Entity cardSlot) { m_currentSlot = cardSlot; }
		void ResetCurrentlSlot(Vec2 prevPos) { m_currentSlot = Entity(); }

	public:
		int m_rank;
		int m_suit;

	private:
		bool m_isHeld = false;
		bool m_isAvailable = true;
		Vec2 m_previousPosition = Vec2(0.0f);
		Entity m_currentSlot;
	};
}
