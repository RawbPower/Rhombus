#pragma once

#include "Component.h"

namespace rhombus
{
	// TODO: Add a Collider2DComponent base class

	class BoxCollider2DComponent : public ComponentBase
	{
	public:
		Vec2 m_offset = { 0.0f, 0.0f };
		Vec2 m_size = { 0.5f, 0.5f };

		// TODO: Move to physics material
		float m_density = 1.0f;
		float m_friction = 0.5f;
		float m_restitution = 0.0f;
		float m_restitutionThreshold = 0.5f;

		// Storage for runtime
		void* m_runtimeFixture = nullptr;

		BoxCollider2DComponent();
		BoxCollider2DComponent(const BoxCollider2DComponent& other) = default;
	};

	class CircleCollider2DComponent : public ComponentBase
	{
	public:
		Vec2 m_offset = { 0.0f, 0.0f };
		float m_radius = 0.5f;

		// TODO: Move to physics material
		float m_density = 1.0f;
		float m_friction = 0.5f;
		float m_restitution = 0.0f;
		float m_restitutionThreshold = 0.5f;

		// Storage for runtime
		void* m_runtimeFixture = nullptr;

		CircleCollider2DComponent() = default;
		CircleCollider2DComponent(const CircleCollider2DComponent& other) = default;
	};
}
