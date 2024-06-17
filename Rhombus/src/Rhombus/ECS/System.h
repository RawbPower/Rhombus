#pragma once

#include "ECSTypes.h"
#include <set>
#include <vector>

namespace rhombus
{
	class Entity;
	class Scene;

	class System
	{
	public:
		System(Scene* scene) : m_scene(scene)
		{
		}

		std::vector<Entity> GetEntities();

		std::set<EntityID> m_entityIDs;
		Scene* m_scene = nullptr;
	};
}
