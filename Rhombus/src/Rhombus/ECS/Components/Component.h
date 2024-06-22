#pragma once

#include "Rhombus/Core/UUID.h"

#include "Rhombus/ECS/Entity.h"

namespace rhombus
{
	class ComponentBase
	{
	public:
		virtual void OnComponentAdded() {}

		void SetOwnerEntity(Entity entity) { m_ownerEntity = entity; }
		void SetOwnerEntity(EntityID entityID, Scene* scene) { m_ownerEntity = { entityID, scene }; }
		Entity GetOwnerEntity() const { return m_ownerEntity; }

	private:
		Entity m_ownerEntity;
	};

	class IDComponent : public ComponentBase
	{
	public:
		UUID m_id;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
		IDComponent(const UUID id) : m_id(id) {}
	};

	class TagComponent : public ComponentBase
	{
	public:
		TagComponent() = default;
		TagComponent(const TagComponent&) = default;

		std::string m_tag;
	private:
		friend class Scene;
	};
}
