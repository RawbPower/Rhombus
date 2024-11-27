#include "rbpch.h"
#include "PixelPlatformerPhysicsSystem.h"
#include "Rhombus/ECS/Components/PixelPlatformerBodyComponent.h"
#include "Rhombus/ECS/Components/TransformComponent.h"


namespace rhombus
{
	const float GRAVITY = -16.0f * 9.8f;			// 16 pixels -> 1 meter

	void PixelPlatformerPhysicsSystem::Update(DeltaTime dt)
	{
		for (Entity entity : GetEntities())
		{
			auto& ppbComponent = entity.GetComponent<PixelPlatformerBodyComponent>();

			switch (ppbComponent.m_type)
			{
			case PixelPlatformerBodyComponent::BodyType::Static:
				break;
			case PixelPlatformerBodyComponent::BodyType::Kinematic:
				break;
			case PixelPlatformerBodyComponent::BodyType::Dynamic:
				UpdateDynamicBody(entity, dt);
				break;
			default:
				break;
			}
		}
	}

	void PixelPlatformerPhysicsSystem::UpdateDynamicBody(Entity entity, DeltaTime dt)
	{
		PixelPlatformerBodyComponent& ppbComponent = entity.GetComponent<PixelPlatformerBodyComponent>();

		ppbComponent.m_velocity += Vec2(0.0f, GRAVITY * dt);

		Vec2 translation = ppbComponent.m_velocity * dt;
		Move(entity, translation);
	}

	void PixelPlatformerPhysicsSystem::Move(Entity entity, Vec2 translation)
	{
		TransformComponent& transformComponent = entity.GetComponent<TransformComponent>();
		PixelPlatformerBodyComponent& ppbComponent = entity.GetComponent<PixelPlatformerBodyComponent>();
		Vec2& remainder = ppbComponent.m_translationRemainder;
		Vec2 appliedTranslation = Vec2(0.0f);

		remainder += translation;
		int xTranslation = math::RoundInt(remainder.x);
		int yTranslation = math::RoundInt(remainder.y);

		if (xTranslation != 0)
		{
			remainder.x -= (float)xTranslation;
			int step = math::Sign(xTranslation);

			while (xTranslation != 0)
			{
				bool collide = false;
				if (!collide)
				{
					appliedTranslation.x += step;
					xTranslation -= step;
				}
				else
				{
					break;
				}
			}
		}

		if (yTranslation != 0)
		{
			remainder.y -= (float)yTranslation;
			int step = math::Sign(yTranslation);

			while (yTranslation != 0)
			{
				bool collide = false;
				if (!collide)
				{
					appliedTranslation.y += step;
					yTranslation -= step;
				}
				else
				{
					break;
				}
			}
		}

		transformComponent.SetWorldPosition(transformComponent.GetWorldPosition() + appliedTranslation);
	}
}