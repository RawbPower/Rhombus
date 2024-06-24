#include "CardPlacementSystem.h"

#include "Patience/Components/CardComponent.h"
#include "Patience/Components/CardSlotComponent.h"

void CardPlacementSystem::Init()
{
	for (Entity cardEntity : GetEntities())
	{
		EntityID currentSlot = CheckForCardSlot(cardEntity);

		CardComponent& card = cardEntity.GetComponent<CardComponent>();
		TransformComponent& cardTransform = cardEntity.GetComponent<TransformComponent>();

		if (currentSlot != INVALID_ENTITY)
		{
			Entity currentSlotEntity = { currentSlot, m_scene };
			CardSlotComponent& cardSlot = currentSlotEntity.GetComponent<CardSlotComponent>();
			cardSlot.AddCard(cardEntity);
			card.SetCurrentlSlot(currentSlotEntity);
		}
	}
}

void CardPlacementSystem::Update(DeltaTime time)
{

}

void CardPlacementSystem::OnMouseMoved(int x, int y)
{
	Vec3 cursorCoords = Renderer2D::ConvertScreenToWorldSpace(x, y);
	for (Entity entity : GetEntities())
	{
		auto& transform = entity.GetComponent<TransformComponent>();
		auto& card = entity.GetComponent<CardComponent>();

		if (card.GetIsHeld())
		{
			transform.m_position = Vec3(cursorCoords.x, cursorCoords.y, transform.m_position.z);
			transform.SetLayer(Z_LAYER::FOREGROUND_2_LAYER);
		}
	}
}

void CardPlacementSystem::OnMouseButtonPressed(int button)
{
	for (Entity entity : GetEntities())
	{
		auto& card = entity.GetComponent<CardComponent>();

		if (entity.HasComponent<BoxArea2DComponent>() && card.GetIsAvailable())
		{
			auto& ba2D = entity.GetComponent<BoxArea2DComponent>();

			if (ba2D.m_isMouseInArea)
			{
				auto& transform = entity.GetComponent<TransformComponent>();
				card.SetIsHeld(true);
				card.SetPreviousPosition(transform.m_position);
			}
		}
	}
}

void CardPlacementSystem::OnMouseButtonReleased(int button)
{
	for (Entity entity : GetEntities())
	{
		auto& card = entity.GetComponent<CardComponent>();

		if (card.GetIsHeld())
		{
			card.SetIsHeld(false);
			PlaceCard(entity);
		}
	}
}

void CardPlacementSystem::PlaceCard(Entity cardEntity)
{
	EntityID currentSlot = CheckForCardSlot(cardEntity);

	CardComponent& card = cardEntity.GetComponent<CardComponent>();
	TransformComponent& cardTransform = cardEntity.GetComponent<TransformComponent>();

	if (currentSlot == INVALID_ENTITY)
	{
		cardTransform.m_position = Vec3(card.GetPreviousPosition(), cardTransform.m_position.z);
	}
	else
	{
		Entity currentSlotEntity = { currentSlot, m_scene };
		TransformComponent& slotTransform = currentSlotEntity.GetComponent<TransformComponent>();
		cardTransform.m_position = Vec3(slotTransform.m_position.x, slotTransform.m_position.y, slotTransform.m_position.z);
		cardTransform.SetLayer(Z_LAYER::FOREGROUND_1_LAYER);
		CardSlotComponent& cardSlot = currentSlotEntity.GetComponent<CardSlotComponent>();
			
		if (cardSlot.CanAcceptCards())
		{
			Entity previousCardSlot = card.GetCurrentSlot();
			previousCardSlot.GetComponent<CardSlotComponent>().RemoveCard(cardEntity);
			cardSlot.AddCard(cardEntity);
			card.SetCurrentlSlot(currentSlotEntity);
		}
		else
		{
			cardTransform.m_position = Vec3(card.GetPreviousPosition(), cardTransform.m_position.z);
		}
	}
}

EntityID CardPlacementSystem::CheckForCardSlot(Entity cardEntity)
{
	EntityID currentSlot = INVALID_ENTITY;
	const CardComponent& card = cardEntity.GetComponent<CardComponent>();
	const BoxArea2DComponent& cardBoxArea = cardEntity.GetComponent<BoxArea2DComponent>();
	TransformComponent& cardTransform = cardEntity.GetComponent<TransformComponent>();
	EntityID nearestCardSlot = -1;
	float nearestSeparation = -1.0f;
	std::vector<EntityID> view = m_scene->GetRegistry().GetEntityList<CardSlotComponent>();
	for (EntityID e : view)
	{
		Entity entity = { e, m_scene };
		const BoxArea2DComponent& slotArea = entity.GetComponent<BoxArea2DComponent>();
		const TransformComponent& slotTransform = entity.GetComponent<TransformComponent>();

		Vec3 cardPos = cardTransform.m_position + Vec3(cardBoxArea.m_offset, 0.0f);
		Vec3 slotPos = slotTransform.m_position + Vec3(slotArea.m_offset, 0.0f);

		Vec2 separation = Vec2(abs(cardPos.x - slotPos.x), abs(cardPos.y - slotPos.y));
		if (separation.GetMagnitude() < nearestSeparation || nearestSeparation < 0.0f)
		{
			nearestCardSlot = entity;
			nearestSeparation = separation.GetMagnitude();
		}
	}

	bool placedInSlot = false;
	if (nearestCardSlot >= 0)
	{
		Entity entity = { nearestCardSlot, m_scene };
		const BoxArea2DComponent& slotArea = entity.GetComponent<BoxArea2DComponent>();
		TransformComponent& slotTransform = entity.GetComponent<TransformComponent>();

		Vec3 cardPos = cardTransform.m_position + Vec3(cardBoxArea.m_offset, 0.0f);
		Vec3 slotPos = slotTransform.m_position + Vec3(slotArea.m_offset, 0.0f);

		Vec2 separation = Vec2(abs(cardPos.x - slotPos.x), abs(cardPos.y - slotPos.y));

		if (separation.x < (slotArea.m_size.x + cardBoxArea.m_size.x) && separation.y < (slotArea.m_size.y + cardBoxArea.m_size.y))
		{
			currentSlot = entity;
		}
	}

	return currentSlot;
}