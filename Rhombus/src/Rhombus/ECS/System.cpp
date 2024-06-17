#include "rbpch.h"
#include "System.h"

#include "Entity.h"

namespace rhombus
{
	std::vector<Entity> System::GetEntities()
	{
		std::vector<Entity> entities;
		for (EntityID id : m_entityIDs)
		{
			entities.push_back({ id, m_scene });
		}

		return entities;
	}
}