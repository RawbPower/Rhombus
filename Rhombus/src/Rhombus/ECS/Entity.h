#pragma once

#include "Scene.h"

#include "entt.hpp"

namespace rhombus
{

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity id, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T>
		bool HasComponent()
		{
			return m_scene->m_Registry.any_of<T>(m_entityId);
		}

		template<typename T>
		const T& GetComponentRead()
		{
			RB_CORE_ASSERT(HasComponent<T>(), "Entity ({0}) does not have component that you are trying to get!", m_entityId);

			return m_scene->m_Registry.get<T>(m_entityId);
		}

		template<typename T>
		T& GetComponent()
		{
			RB_CORE_ASSERT(HasComponent<T>(), "Entity ({0}) does not have component that you are trying to get!", m_entityId);

			return m_scene->m_Registry.get<T>(m_entityId);
		}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			RB_CORE_ASSERT(!HasComponent<T>(), "Entity ({0}) already has component that is being added!", m_entityId);

			return m_scene->m_Registry.emplace<T>(m_entityId, std::forward<Args>(args)...);
		}

		template<typename T>
		void RemoveComponent()
		{
			RB_CORE_ASSERT(HasComponent<T>(), "Entity ({0}) does not have component trying to be removed!", m_entityId);

			return m_scene->m_Registry.remove<T>(m_entityId);
		}
		
		operator bool() const { return m_entityId != entt::null; }

	private:
		entt::entity m_entityId = entt::null;
		Scene* m_scene = nullptr;
	};
}
