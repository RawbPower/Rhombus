#pragma once

#include "Rhombus.h"

namespace rhombus
{
	class CardComponent
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

	public:
		int m_rank;
		int m_suit;

	private:
		bool m_isHeld = false;
	};
}
