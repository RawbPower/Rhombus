#include "rbpch.h"
#include "PixelPlatformerPhysicsSystem.h"
#include "Rhombus/ECS/Components/Collider2DComponent.h"
#include "Rhombus/ECS/Components/PixelPlatformerBodyComponent.h"
#include "Rhombus/ECS/Components/TransformComponent.h"
#include "Rhombus/Physics/AABB.h"
#include "Rhombus/Physics/PrimitiveTests.h"

namespace rhombus
{
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

		bool isInAir = ppbComponent.m_isInAir;

		// We are going to step forward per pixel. We can afford to do this
		// since there are generally very few entities simulating on screen
		// Note: We can optimize as it become required

		// X direction
		if (xTranslation != 0)
		{
			// Remove the whole translation from the remainer once we move (even if we collide)
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
					ppbComponent.m_velocity.x = 0.0f;
					break;
				}
			}
		}

		// Y direction
		if (yTranslation != 0)
		{
			isInAir = true;

			// Remove the whole translation from the remainer once we move (even if we collide)
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
					ppbComponent.m_velocity.y = 0.0f;
					isInAir = false;
					break;
				}
			}
		}

		ppbComponent.m_isInAir = isInAir;
		transformComponent.SetWorldPosition(transformComponent.GetWorldPosition() + appliedTranslation);
	}

	bool PixelPlatformerPhysicsSystem::Collide(Entity entity, Vec2 position)
	{
		for (Entity other : GetEntities())
		{
			if (entity == other)
			{
				continue;
			}

			// We only want to collide with static objects (for now)
			PixelPlatformerBodyComponent& otherPhysicsBody = other.GetComponent<PixelPlatformerBodyComponent>();
			if (otherPhysicsBody.m_type != PixelPlatformerBodyComponent::BodyType::Static)
			{
				continue;
			}

			BoxCollider2DComponent& otherColliderComponent = other.GetComponent<BoxCollider2DComponent>();

			TransformComponent& otherTransformComponent = other.GetComponent<TransformComponent>();
			AABB otherAABB;
			otherAABB.c = otherTransformComponent.GetWorldPosition() + otherColliderComponent.m_offset;
			otherAABB.r = Vec3(otherColliderComponent.m_size.x, otherColliderComponent.m_size.y, 10.0f);

			// Check the type of collider that the dyanamic body has, and do the appropriate test
			if (entity.HasComponent<BoxCollider2DComponent>())
			{
				BoxCollider2DComponent& myColliderComponent = entity.GetComponent<BoxCollider2DComponent>();
				AABB myAABB;
				myAABB.c = Vec3(position.x, position.y, 0.0f) + myColliderComponent.m_offset;
				myAABB.r = Vec3(myColliderComponent.m_size.x, myColliderComponent.m_size.y, 10.0f);

				if (AABB::TestAABBAABB(myAABB, otherAABB))
				{
					return true;
				}
			}
			else if (entity.HasComponent<CircleCollider2DComponent>())
			{
				CircleCollider2DComponent& myColliderComponent = entity.GetComponent<CircleCollider2DComponent>();
				Sphere mySphere;
				mySphere.c = Vec3(position.x, position.y, 0.0f) + myColliderComponent.m_offset;
				mySphere.r = myColliderComponent.m_radius;

				if (PrimitiveTests::TestSphereAABB(mySphere, otherAABB))
				{
					return true;
				}
			}
		}

		return false;
	}
}