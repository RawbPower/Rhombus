#pragma once

#include "Rhombus/ECS/ECSTypes.h"
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
		bool HasComponent() const
		{
			return m_scene->m_Registry.HasComponent<T>(m_entityId);
		}

		template<typename T>
		const T& GetComponentRead() const
		{
			Log::Assert(HasComponent<T>(), "Entity ({0}) does not have component that you are trying to get!", m_entityId);

			return m_scene->m_Registry.GetComponent<T>(m_entityId);
		}

		template<typename T>
		T& GetComponent()
		{
			Log::Assert(HasComponent<T>(), "Entity ({0}) does not have component that you are trying to get!", m_entityId);

			return m_scene->m_Registry.GetComponent<T>(m_entityId);
		}

		template<typename T>
		T& AddComponent()
		{
			Log::Assert(!HasComponent<T>(), "Entity ({0}) already has component that is being added!", m_entityId);
			//T& component = m_scene->m_Registry.emplace<T>(m_entityId, std::forward<Args>(args)...);
			T& component = m_scene->m_Registry.AddComponent<T>(m_entityId);
			component.SetOwnerEntity(*this);
			component.OnComponentAdded();
			return component;
		}

		template<typename T>
		T& AddComponent(std::function<void(T&)> OnCreated)
		{
			Log::Assert(!HasComponent<T>(), "Entity ({0}) already has component that is being added!", m_entityId);
			//T& component = m_scene->m_Registry.emplace<T>(m_entityId, std::forward<Args>(args)...);
			T& component = m_scene->m_Registry.AddComponent<T>(m_entityId);
			OnCreated(component);
			component.SetOwnerEntity(*this);
			component.OnComponentAdded();
			return component;
		}

		template<typename T>
		T& AddComponent(T srcComponent)
		{
			Log::Assert(!HasComponent<T>(), "Entity ({0}) already has component that is being added!", m_entityId);
			//T& component = m_scene->m_Registry.emplace<T>(m_entityId, std::forward<Args>(args)...);
			T& component = m_scene->m_Registry.AddComponent<T>(m_entityId, srcComponent);
			component.SetOwnerEntity(*this);
			component.OnComponentAdded();
			return component;
		}

		template<typename T>
		T& GetOrAddComponent()
		{
			if (HasComponent<T>())
			{
				return GetComponent<T>();
			}
			else
			{
				return AddComponent<T>();
			}
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
			Log::Assert(HasComponent<T>(), "Entity ({0}) does not have component trying to be removed!", m_entityId);

			m_scene->m_Registry.RemoveComponent<T>(m_entityId);
		}

		bool operator==(const Entity& other) const { return m_entityId == other.m_entityId && m_scene == other.m_scene; }

		bool operator!=(const Entity & other) const { return !(*this == other); }
		
		operator bool() const { return m_entityId != -1; }
		operator EntityID() const { return m_entityId; }

		UUID GetUUID() const;
		const std::string GetName() const;
		Mat4 GetTransform() const;
		Scene* GetContext() const { return m_scene; }

		bool IsValid() const { return m_entityId != INVALID_ENTITY; }

		Ref<SceneGraphNode> GetSceneGraphNode() const;

	private:
		EntityID m_rbEntityId;
		EntityID m_entityId = 0xFFFFFFFF;
		Scene* m_scene = nullptr;
	};
}
