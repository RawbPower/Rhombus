#pragma once

#include "ECSTypes.h"
#include <set>


namespace rhombus
{
	class System
	{
	public:
		std::set<EntityID> m_entities;
	};
}
