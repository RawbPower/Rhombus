#pragma once

#include "ECSTypes.h"
#include "Rhombus/Core/UUID.h"
#include "Scene.h"

namespace rhombus
{

	class Entity
	{
	public:
		Entity() = default;
		Entity(EntityID id, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T>
		bool HasComponent()
		{
			return m_scene->m_Registry.HasComponent<T>(m_entityId);
		}

		template<typename T>
		const T& GetComponentRead()
		{
			RB_CORE_ASSERT(HasComponent<T>(), "Entity ({0}) does not have component that you are trying to get!", m_entityId);

			return m_scene->m_Registry.GetComponent<T>(m_entityId);
		}

		template<typename T>
		T& GetComponent()
		{
			RB_CORE_ASSERT(HasComponent<T>(), "Entity ({0}) does not have component that you are trying to get!", m_entityId);

			return m_scene->m_Registry.GetComponent<T>(m_entityId);
		}

		template<typename T>
		T& AddComponent()
		{
			RB_CORE_ASSERT(!HasComponent<T>(), "Entity ({0}) already has component that is being added!", m_entityId);
			//T& component = m_scene->m_Registry.emplace<T>(m_entityId, std::forward<Args>(args)...);
			T& component = m_scene->m_Registry.AddComponent<T>(m_entityId);
			component.SetOwnerEntity(*this);
			component.OnComponentAdded();
			return component;
		}

		template<typename T>
		T& AddComponent(T srcComponent)
		{
			RB_CORE_ASSERT(!HasComponent<T>(), "Entity ({0}) already has component that is being added!", m_entityId);
			//T& component = m_scene->m_Registry.emplace<T>(m_entityId, std::forward<Args>(args)...);
			T& component = m_scene->m_Registry.AddComponent<T>(m_entityId, srcComponent);
			component.SetOwnerEntity(*this);
			component.OnComponentAdded();
			return component;
		}

		template<typename T>
		T& AddOrReplaceComponent(T srcComponent)
		{
			//T& component = m_scene->m_Registry.emplace_or_replace<T>(m_entityId, std::forward<Args>(args)...);
			T& component = m_scene->m_Registry.AddOrReplaceComponent<T>(m_entityId, srcComponent);
			component.SetOwnerEntity(*this);
			component.OnComponentAdded();
			return component;
		}

		template<typename T>
		void RemoveComponent()
		{
			RB_CORE_ASSERT(HasComponent<T>(), "Entity ({0}) does not have component trying to be removed!", m_entityId);

			m_scene->m_Registry.RemoveComponent<T>(m_entityId);
		}

		bool operator==(const Entity& other) const { return m_entityId == other.m_entityId && m_scene == other.m_scene; }

		bool operator!=(const Entity & other) const { return !(*this == other); }
		
		operator bool() const { return m_entityId != -1; }
		operator EntityID() const { return m_entityId; }

		UUID GetUUID();
		const std::string GetName();

	private:
		EntityID m_rbEntityId;
		EntityID m_entityId = -1;
		Scene* m_scene = nullptr;
	};
}
