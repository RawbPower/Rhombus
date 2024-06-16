#pragma once

#include "ECSTypes.h"
#include <set>

namespace rhombus
{
	class Scene;

	class System
	{
	public:
		System(Scene* scene) : m_scene(scene)
		{
		}

		std::set<EntityID> m_entities;
		Scene* m_scene = nullptr;
	};
}
