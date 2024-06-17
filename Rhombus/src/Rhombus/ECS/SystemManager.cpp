#include "rbpch.h"
#include "SystemManager.h"

namespace rhombus
{
	void SystemManager::OnEntityDestroyed(EntityID entity)
	{
		// Erase a destroyed entity from all system lists
		// m_entities is a set so no check needed
		for (auto const& pair : m_systems)
		{
			auto const& system = pair.second;

			system->m_entityIDs.erase(entity);
		}
	}

	void SystemManager::OnEntitySignatureChanged(EntityID entity, Signature entitySignature)
	{
		// Notify each system that an entity's signature changed
		for (auto const& pair : m_systems)
		{
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = m_signatures[type];

			// Entity signature matches system signature - insert into set
			if ((entitySignature & systemSignature) == systemSignature)
			{
				system->m_entityIDs.insert(entity);		// Because this is a set we don't need to check if it is already included
			}
			// Entity signature does not match system signature - erase from set
			else
			{
				system->m_entityIDs.erase(entity);		// Because this is a set we don't need to check if the entity is in it
			}
		}
	}
}