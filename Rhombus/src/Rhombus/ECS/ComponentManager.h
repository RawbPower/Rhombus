#pragma once

#include "ECSTypes.h"
#include "ComponentArray.h"
#include "Rhombus/Core/Log.h"

#include <unordered_map>

namespace rhombus
{
	class ComponentManager
	{
	public:

		template<typename T>
		void RegisterComponent()
		{
			const char* typeName = typeid(T).name();

			RB_CORE_ASSERT(m_componentTypes.find(typeName) == m_componentTypes.end(), "Registering component type more than once.");

			// Add this component type to the component type map
			m_componentTypes.insert({ typeName, m_nextComponentType });

			// Create a ComponentArray pointer and add it to the component arrays map
			m_componentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });

			// Incremenmt the value so that the next component registered will be different
			m_nextComponentType++;
		}

		template<typename T>
		ComponentType GetComponentType()
		{
			const char* typeName = typeid(T).name();

			RB_CORE_ASSERT(m_componentTypes.find(typeName) != m_componentTypes.end(), "Component not registered before use.");

			// Return this component's type - used for creating signatures
			return m_componentTypes[typeName];
		}

		template<typename T>
		T& AddComponent(EntityID entity)
		{
			// Add component to the array for an entity
			return GetComponentArray<T>()->InsertData(entity);
		}

		template<typename T>
		T& AddComponent(EntityID entity, T component)
		{
			// Add component to the array for an entity
			return GetComponentArray<T>()->InsertData(entity, component);
		}

		template<typename T>
		T& ReplaceComponent(EntityID entity, T component)
		{
			// Replace component in the array for an entity
			return GetComponentArray<T>()->ReplaceData(entity, component);
		}

		template<typename T>
		void RemoveComponent(EntityID entity)
		{
			// Remove component to the array for an entity
			GetComponentArray<T>()->RemoveData(entity);
		}

		template<typename T>
		T& GetComponent(EntityID entity)
		{
			// Get a reference to a component from the array for an entity
			return GetComponentArray<T>()->GetData(entity);
		}

		template<typename T>
		bool HasComponent(EntityID entity)
		{
			// Check if the component is in the array for an entity
			return GetComponentArray<T>()->HasData(entity);
		}

		template<typename T>
		std::vector<EntityID> GetEntityList()
		{
			return GetComponentArray<T>()->GetEntityList();
		}

		template<typename T>
		EntityID GetFirstEntity()
		{
			return GetComponentArray<T>()->GetFirstEntity();
		}

		void OnEntityDestroyed(EntityID entity)
		{
			// Notify each component array that an entity has been destoryed
			// If it has a component for that entity, it will remove it
			for (auto const& pair : m_componentArrays)
			{
				auto const& component = pair.second;

				component->OnEntityDestroyed(entity);
			}
		}

	private:
		// Map from the type string point to the component type
		std::unordered_map<const char*, ComponentType> m_componentTypes{};

		// Map from type string point to component array
		std::unordered_map<const char*, Ref<ComponentArrayBase>> m_componentArrays{};

		// The component type to be assigned to the next registered component - starting at 0
		ComponentType m_nextComponentType{};

		// Convenience function to get the statically casted pointer to the CopmonentArray of type T
		template<typename T>
		Ref<ComponentArray<T>> GetComponentArray()
		{
			const char* typeName = typeid(T).name();

			RB_CORE_ASSERT(m_componentTypes.find(typeName) != m_componentTypes.end(), "Component not registered before use.");

			return std::static_pointer_cast<ComponentArray<T>>(m_componentArrays[typeName]);
		}
	};
}
