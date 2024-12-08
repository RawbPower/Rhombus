#include "rbpch.h"
#include "EntityManager.h"

#include "Rhombus/Core/Log.h"

namespace rhombus
{
	EntityManager::EntityManager()
	{
		// Initialize the queue with all possible entity IDs
		for (EntityID entity = 0; entity < MAX_ENTITIES; ++entity)
		{
			m_availableEntities.push(entity);
		}
	}

	EntityID EntityManager::CreateEntity()
	{
		Log::Assert(m_activeEntityCount < MAX_ENTITIES, "Too many enities in exitence.");

		// Take an ID from the front of the queue
		EntityID id = m_availableEntities.front();
		m_availableEntities.pop();
		m_activeEntityCount++;

		return id;
	}

	void EntityManager::DestroyEntity(EntityID entity)
	{
		Log::Assert(entity < MAX_ENTITIES, "Entity out of range.");

		// Invalidate the destroyed entity's signature
		m_signatures[entity].reset();

		// Put the destroyed ID at the back of the queue
		m_availableEntities.push(entity);
		m_activeEntityCount--;
	}

	void EntityManager::SetSignature(EntityID entity, Signature signature)
	{
		Log::Assert(entity < MAX_ENTITIES, "Entity out of range.");

		// Put this entity's signature into the array
		m_signatures[entity] = signature;
	}

	Signature EntityManager::GetSignature(EntityID entity)
	{
		Log::Assert(entity < MAX_ENTITIES, "Entity out of range.");

		return m_signatures[entity];
	}
}