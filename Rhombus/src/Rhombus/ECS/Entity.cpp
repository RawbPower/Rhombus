#include "rbpch.h"
#include "Entity.h"

#include "Components/Component.h"

namespace rhombus
{
	Entity::Entity(EntityID id, Scene* scene)
		: m_entityId(id), m_scene(scene)
	{

	}

	UUID Entity::GetUUID() 
	{ 
		return GetComponent<IDComponent>().m_id; 
	}

	const std::string Entity::GetName() 
	{ 
		return GetComponent<TagComponent>().m_tag; 
	}
}