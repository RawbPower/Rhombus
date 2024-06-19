#pragma once

#include "Rhombus.h"

#include <list>

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

		void AddCard(Entity card) { m_cardStack.push_back(card); m_isOccupied = true; }
		void RemoveCard(Entity card) { m_cardStack.remove(card); }

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

	public:
		std::list<Entity> m_cardStack;

	private:
		SlotType m_slotType = SINGLE;
		bool m_isOccupied = false;
	};
}
