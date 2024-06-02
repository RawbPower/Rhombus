#pragma once

#include "ECSTypes.h"
#include "System.h"
#include "Rhombus/Core/Log.h"

#include <unordered_map>

namespace rhombus
{
	class SystemManager
	{
	public:
		template<typename T>
		Ref<T> RegsiterSystem()
		{
			const char* typeName = typeid(T).name();

			RB_CORE_ASSERT(m_systems.find(typeName) == m_systems.end(), "Register system more than once.");

			// Create a pointer to the system and return it so it can be used externally
			auto system = std::make_shared<T>();
			m_systems.insert({ typeName, system });
			return system;
		}

		template<typename T>
		void SetSignature(Signature signature)
		{
			const char* typeName = typeid(T).name();

			RB_CORE_ASSERT(m_systems.find(typeName) != m_systems.end(), "System used before registered.");

			// Set the signature for this system
			m_signatures({ typeName, signature });
		}

		void OnEntityDestroyed(EntityID entity)
		{
			// Erase a destroyed entity from all system lists
			// m_entities is a set so no check needed
			for (auto const& pair : m_systems)
			{
				auto const& system = pair.second;

				system->m_entities.erase(entity);
			}
		}

		void OnEntitySignatureChanged(EntityID entity, Signature entitySignature)
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
					system->m_entities.insert(entity);		// Because this is a set we don't need to check if it is already included
				}
				// Entity signature does not match system signature - erase from set
				else
				{
					system->m_entities.erase(entity);		// Because this is a set we don't need to check if the entity is in it
				}
			}
		}

	private:
		// Map from system type string pointer to a signture
		std::unordered_map<const char*, Signature> m_signatures{};

		// Map from system type string pointer to a system pointer
		std::unordered_map<const char*, Ref<System>> m_systems{};
	};
}
