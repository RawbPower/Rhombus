#include "rbpch.h"
#include "PixelPlatformerPhysicsSystem.h"
#include "Rhombus/ECS/Components/Collider2DComponent.h"
#include "Rhombus/ECS/Components/PixelPlatformerBodyComponent.h"
#include "Rhombus/ECS/Components/TransformComponent.h"
#include "Rhombus/Physics/AABB.h"

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
		Vec3 position = transformComponent.GetWorldPosition();
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
				if (!Collide(entity, position + appliedTranslation + Vec2(step, 0.0f)))
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
				if (!Collide(entity, position + appliedTranslation + Vec2(0.0f, step)))
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

	bool PixelPlatformerPhysicsSystem::Collide(Entity entity, Vec2 position)
	{
		BoxCollider2DComponent& myColliderComponent = entity.GetComponent<BoxCollider2DComponent>();
		AABB myAABB;
		myAABB.c = Vec3(position.x, position.y, 0.0f) + myColliderComponent.m_offset;
		myAABB.r = Vec3(myColliderComponent.m_size.x, myColliderComponent.m_size.y, 10.0f);
		for (Entity other : GetEntities())
		{
			BoxCollider2DComponent& otherColliderComponent = other.GetComponent<BoxCollider2DComponent>();

			if (&myColliderComponent == &otherColliderComponent)
			{
				continue;
			}

			TransformComponent& otherTransformComponent = other.GetComponent<TransformComponent>();
			AABB otherAABB;
			otherAABB.c = otherTransformComponent.GetWorldPosition() + otherColliderComponent.m_offset;
			otherAABB.r = Vec3(otherColliderComponent.m_size.x, otherColliderComponent.m_size.y, 10.0f);

			if (AABB::TestAABBAABB(myAABB, otherAABB))
			{
				return true;
			}
		}

		return false;
	}
}