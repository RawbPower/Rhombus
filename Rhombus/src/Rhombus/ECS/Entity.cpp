#include "rbpch.h"
#include "Entity.h"

namespace rhombus
{
	Entity::Entity(entt::entity id, Scene* scene)
		: m_entityId(id), m_scene(scene)
	{

	}
}