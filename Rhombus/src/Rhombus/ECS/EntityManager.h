#pragma once

#include "ECSTypes.h"
#include "Rhombus/Core/Log.h"

#include <array>
#include <queue>

namespace rhombus
{
	class EntityManager
	{
	public:
		EntityManager()
		{
			// Initialize the queue with all possible entity IDs
			for (EntityID entity = 0; entity < MAX_ENTITIES; ++entity)
			{
				m_availableEntities.push(entity);
			}
		}

		EntityID CreateEntity()
		{
			RB_CORE_ASSERT(m_activeEntityCount < MAX_ENTITIES, "Too many enities in exitence.");

			// Take an ID from the front of the queue
			EntityID id = m_availableEntities.front();
			m_availableEntities.pop();
			m_activeEntityCount++;

			return id;
		}

		void DestroyEntity(EntityID entity)
		{
			RB_CORE_ASSERT(entity < MAX_ENTITIES, "Entity out of range.");

			// Invalidate the destroyed entity's signature
			m_signatures[entity].reset();

			// Put the destroyed ID at the back of the queue
			m_availableEntities.push(entity);
			m_activeEntityCount--;
		}

		void SetSignature(EntityID entity, Signature signature)
		{
			RB_CORE_ASSERT(entity < MAX_ENTITIES, "Entity out of range.");

			// Put this entity's signature into the array
			m_signatures[entity] = signature;
		}

		Signature GetSignature(EntityID entity)
		{
			RB_CORE_ASSERT(entity < MAX_ENTITIES, "Entity out of range.");

			return m_signatures[entity];
		}
	private:
		// Queue of unused entity IDs
		std::queue<EntityID> m_availableEntities{};

		// Array of signatures where the index corresponds to the entity ID
		std::array<Signature, MAX_ENTITIES> m_signatures{};

		// Total active entities - used to keep limits on how many exist
		uint32_t m_activeEntityCount{};
	};
}
