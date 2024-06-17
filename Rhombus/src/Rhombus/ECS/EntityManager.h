#pragma once

#include "ECSTypes.h"

#include <array>
#include <queue>

namespace rhombus
{
	class EntityManager
	{
	public:
		EntityManager();

		EntityID CreateEntity();

		void DestroyEntity(EntityID entity);

		void SetSignature(EntityID entity, Signature signature);

		Signature GetSignature(EntityID entity);
	private:
		// Queue of unused entity IDs
		std::queue<EntityID> m_availableEntities{};

		// Array of signatures where the index corresponds to the entity ID
		std::array<Signature, MAX_ENTITIES> m_signatures{};

		// Total active entities - used to keep limits on how many exist
		uint32_t m_activeEntityCount{};
	};
}
