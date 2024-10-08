#include "rbpch.h"
#include "SceneGraphNode.h"

#include "Rhombus/ECS/ECSTypes.h"

namespace rhombus
{
	SceneGraphNode::SceneGraphNode() : SceneGraphNode(Entity(INVALID_ENTITY, nullptr))
	{

	}

	SceneGraphNode::SceneGraphNode(Entity entity)
		: m_entity(entity), m_parent(this), m_isDirty(true), m_localTransform(Mat4::Identity()), m_worldTransform(Mat4::Identity())
	{

	}

	Ref<SceneGraphNode> SceneGraphNode::AddChild(Entity entity)
	{
		m_children.push_back(CreateRef<SceneGraphNode>(entity));
		m_children.back()->SetParent(this);
		return m_children.back();
	}

	void SceneGraphNode::AddChild(Ref<SceneGraphNode> sceneGraphNode)
	{
		sceneGraphNode->GetParent()->RemoveChild(sceneGraphNode);
		m_children.push_back(sceneGraphNode);
		sceneGraphNode->SetParent(this);
	}

	void SceneGraphNode::RemoveChild(Ref<SceneGraphNode> sceneGraphNode)
	{
		for (int i = 0; i < m_children.size(); i++)
		{
			if (m_children[i] == sceneGraphNode)
			{
				m_children.erase(m_children.begin() + i);
				break;
			}
		}
	}

	void SceneGraphNode::Update()
	{
		if ((EntityID)m_entity != INVALID_ENTITY)
		{
			m_localTransform = m_entity.GetTransform();
		}

		if (m_parent)
		{
			m_worldTransform = m_parent->GetWorldTransform() * m_localTransform;
		}
		else
		{
			m_worldTransform = m_localTransform;
		}

		for (auto& child : m_children)
		{
			child->Update();
		}
	}
}