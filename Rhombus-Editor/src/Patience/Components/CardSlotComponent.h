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

		virtual void OnComponentAdded() override;

		int GetIsOccupied() const { return m_isOccupied; }
		void SetIsOccupied(bool occupied) { m_isOccupied = occupied; }

		bool CanAcceptCards();
		void AddCard(Entity card);
		void RemoveCard(Entity card);

		void UpdateCardStack();

		void SetSlotType(int slotType)
		{
			m_slotType = (SlotType)slotType;
		}

		Vec2& GetStaggeredOffset() { return m_staggeredOffset; }
		void SetStaggeredOffset(Vec2 offset) { m_staggeredOffset = offset; }

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
		Vec2 m_staggeredOffset = Vec2(0.0f, -16.0f);

	private:
		SlotType m_slotType = SINGLE;
		bool m_isOccupied = false;

		Vec2 m_emptyAreaOffset = { 0.0f, 0.0f };
		Vec2 m_emptyAreaSize = { 0.5f, 0.5f };
	};
}
