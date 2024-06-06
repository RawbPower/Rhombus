#pragma once

#include "ECSTypes.h"
#include "Rhombus/Core/Log.h"

#include <array>
#include <unordered_map>

namespace rhombus
{
	// An interface is needed so that the ComponentManager (seen later)
	// can tell a generic ComponentArray that an entity has been destroyed
	// and that it needs to update its array mappings.
	class ComponentArrayBase
	{
	public:
		virtual ~ComponentArrayBase() = default;
		virtual void OnEntityDestroyed(EntityID entity) = 0;
	};

	template<typename T>
	class ComponentArray : public ComponentArrayBase
	{
	public:
		T& InsertData(EntityID entity)
		{
			RB_CORE_ASSERT(m_entityToIndexMap.find(entity) == m_entityToIndexMap.end(), "Component added to same entity more than once.");

			// Put new entry at the end and update the maps
			size_t newIndex = m_size;
			m_entityToIndexMap[entity] = newIndex;
			m_indexToEntityMap[newIndex] = entity;
			m_size++;
			return m_componentArray[newIndex];
		}

		T& InsertData(EntityID entity, T component)
		{
			RB_CORE_ASSERT(m_entityToIndexMap.find(entity) == m_entityToIndexMap.end(), "Component added to same entity more than once.");

			// Put new entry at the end and update the maps
			size_t newIndex = m_size;
			m_entityToIndexMap[entity] = newIndex;
			m_indexToEntityMap[newIndex] = entity;
			m_componentArray[newIndex] = component;
			m_size++;
			return m_componentArray[newIndex];
		}

		// Is there too much copying when using registry wrapper?
		T& ReplaceData(EntityID entity, T component)
		{
			RB_CORE_ASSERT(m_entityToIndexMap.find(entity) != m_entityToIndexMap.end(), "Replacing non-existent component.");

			m_componentArray[m_entityToIndexMap[entity]] = component;
			return m_componentArray[m_entityToIndexMap[entity]];
		}

		void RemoveData(EntityID entity)
		{
			RB_CORE_ASSERT(m_entityToIndexMap.find(entity) != m_entityToIndexMap.end(), "Removing non-existent component.");

			// Copy element at end into deleted element's place to maintain density
			size_t indexOfRemovedEntity = m_entityToIndexMap[entity];
			size_t indexOfLastEntity = m_size - 1;
			m_componentArray[indexOfRemovedEntity] = m_componentArray[indexOfLastEntity];

			// Update map to point to the moved spot
			EntityID entityOfLastElement = m_indexToEntityMap[indexOfLastEntity];
			m_entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
			m_indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

			m_entityToIndexMap.erase(entity);
			m_indexToEntityMap.erase(indexOfLastEntity);

			m_size--;
		}

		T& GetData(EntityID entity)
		{
			RB_CORE_ASSERT(m_entityToIndexMap.find(entity) != m_entityToIndexMap.end(), "Retrieving non-existent component.");

			// Return a reference to the entity's component
			return m_componentArray[m_entityToIndexMap[entity]];
		}

		bool HasData(EntityID entity)
		{
			// Return whether this entity has component T
			return m_entityToIndexMap.find(entity) != m_entityToIndexMap.end();
		}

		std::vector<EntityID> GetEntityList()
		{
			std::vector<EntityID> list;
			for (auto const& pair : m_entityToIndexMap)
			{
				list.push_back(pair.first);
			}
			return list;
		}

		void OnEntityDestroyed(EntityID entity) override
		{
			if (m_entityToIndexMap.find(entity) != m_entityToIndexMap.end())
			{
				// Remove the entity's component if it existed
				RemoveData(entity);
			}
		}

	private:
		// The packed array of components (of generic type T),
		// set to a specified maximum amount, mathcing the maximum number
		// of entities allowed to exist simultaneously, so that each entity
		// has a unique spot
		std::array<T, MAX_ENTITIES> m_componentArray;

		// Map from an entity ID to an array index
		std::unordered_map<EntityID, size_t> m_entityToIndexMap;

		// Map from an array index to an entity ID
		std::unordered_map<size_t, EntityID> m_indexToEntityMap;

		// Total size of valid entries in the array
		size_t m_size;
	};
}
