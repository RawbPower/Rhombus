#include "rbpch.h"
#include "Entity.h"

#include "Rhombus/ECS/Components/Component.h"
#include "Rhombus/ECS/Components/TransformComponent.h"

namespace rhombus
{
	Entity::Entity(EntityID id, Scene* scene)
		: m_entityId(id), m_scene(scene)
	{

	}

	UUID Entity::GetUUID() const
	{ 
		return GetComponentRead<IDComponent>().m_id; 
	}

	const std::string Entity::GetName() const
	{ 
		return GetComponentRead<TagComponent>().m_tag; 
	}

	Mat4 Entity::GetTransform() const
	{
		return GetComponentRead<TransformComponent>().GetTransform();
	}

	Ref<SceneGraphNode> Entity::GetSceneGraphNode() const
	{
		return GetComponentRead<TransformComponent>().m_sceneGraphNode;
	}
}