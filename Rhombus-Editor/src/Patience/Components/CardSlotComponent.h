#pragma once

#include "Rhombus.h"

namespace rhombus
{
	class CardSlotComponent : public ComponentBase
	{
	public:
		enum SlotType {SINGLE = 0, STACK, STAGGERED, COUNT};

		CardSlotComponent() = default;
		CardSlotComponent(const CardSlotComponent&) = default;

		int GetIsOccupied() const { return m_isOccupied; }
		void SetIsOccupied(bool occupied) { m_isOccupied = occupied; }

		void SetSlotType(int slotType)
		{
			m_slotType = (SlotType)slotType;
		}

		const SlotType GetSlotType() const{ return m_slotType; }

		const char* GetSlotTypeName()
		{
			return GetSlotTypeName(m_slotType);
		}

		const char* GetSlotTypeName(int slotType)
		{
			switch ((SlotType)slotType)
			{
			case rhombus::CardSlotComponent::SINGLE:
				return "Single";
				break;
			case rhombus::CardSlotComponent::STACK:
				return "Stack";
				break;
			case rhombus::CardSlotComponent::STAGGERED:
				return "Staggered";
				break;
			default:
				return "Invalid";
				break;
			}
		}

	private:
		SlotType m_slotType = SINGLE;
		bool m_isOccupied = false;
	};
}
