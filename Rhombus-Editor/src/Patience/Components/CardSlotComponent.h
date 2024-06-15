#pragma once

#include "Rhombus.h"

namespace rhombus
{
	class CardSlotComponent : public ComponentBase
	{
	public:
		CardSlotComponent() = default;
		CardSlotComponent(const CardSlotComponent&) = default;

		int GetIsOccupied() const { return m_isOccupied; }
		void SetIsOccupied(bool occupied) { m_isOccupied = occupied; }

	private:
		bool m_isOccupied = false;
	};
}
