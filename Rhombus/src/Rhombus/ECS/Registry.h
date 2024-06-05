#pragma once

#include "ECSTypes.h"
#include "ComponentManager.h"
#include "EntityManager.h"
#include "SystemManager.h"

namespace rhombus
{
	class Registry
	{
	public:
		void Init()
		{
			// Create pointers to each manager
			m_componentManager = std::make_unique<ComponentManager>();
			m_entityManager = std::make_unique<EntityManager>();
			m_systemManager = std::make_unique<SystemManager>();
		}

		void CopyComponents(const Registry& srcRegistry)
		{
			m_componentManager->CopyComponents(srcRegistry.m_componentManager->GetComponentArrays());
		}

		// Entity methods
		EntityID CreateEntity()
		{
			return m_entityManager->CreateEntity();
		}

		void DestroyEntity(EntityID entity)
		{
			m_entityManager->DestroyEntity(entity);
			m_componentManager->OnEntityDestroyed(entity);
			m_systemManager->OnEntityDestroyed(entity);
		}
		
		// Component methods
		template <typename T>
		void RegisterComponent()
		{
			m_componentManager->RegisterComponent<T>();
		}

		template<typename T>
		T& AddComponent(EntityID entity)
		{
			T& component = m_componentManager->AddComponent<T>(entity);

			auto signature = m_entityManager->GetSignature(entity);
			signature.set(m_componentManager->GetComponentType<T>(), true);
			m_entityManager->SetSignature(entity, signature);

			m_systemManager->OnEntitySignatureChanged(entity, signature);
			return component;
		}

		template<typename T>
		T& AddComponent(EntityID entity, T srcComponent)
		{
			T& component = m_componentManager->AddComponent<T>(entity, srcComponent);

			auto signature = m_entityManager->GetSignature(entity);
			signature.set(m_componentManager->GetComponentType<T>(), true);
			m_entityManager->SetSignature(entity, signature);

			m_systemManager->OnEntitySignatureChanged(entity, signature);
			return component;
		}

		template<typename T>
		T& AddOrReplaceComponent(EntityID entity, T component)
		{
			if (HasComponent<T>(entity))
			{
				return m_componentManager->ReplaceComponent<T>(entity, component);
			}
			else
			{
				return AddComponent<T>(entity, component);
			}
		}

		template<typename T>
		void RemoveComponent(EntityID entity)
		{
			m_componentManager->RemoveComponent<T>(entity);

			auto signature = m_entityManager->GetSignature(entity);
			signature.set(m_componentManager->GetComponentType<T>(), false);
			m_entityManager->SetSignature(entity, signature);

			m_systemManager->OnEntitySignatureChanged(entity, signature);
		}

		template<typename T>
		T& GetComponent(EntityID entity) const
		{
			return m_componentManager->GetComponent<T>(entity);
		}

		template<typename T>
		bool HasComponent(EntityID entity) const
		{
			return m_componentManager->HasComponent<T>(entity);
		}

		template<typename T>
		ComponentType GetComponentType() const
		{
			return m_componentManager->GetComponentType<T>();
		}

		template<typename T>
		std::vector<EntityID> GetEntityList() const
		{
			return m_componentManager->GetEntityList<T>();
		}

		// System methods
		template<typename T>
		Ref<T> RegisterSystem()
		{
			return m_systemManager->RegsiterSystem<T>();
		}

		template<typename T>
		void SetSystemSignature(Signature signature)
		{
			m_systemManager->SetSignature<T>(signature);
		}

	private:
		Scope<ComponentManager> m_componentManager;
		Scope<EntityManager> m_entityManager;
		Scope<SystemManager> m_systemManager;
	};
}
