#pragma once

#include "Entity.h"
#include "Rhombus/Core/Core.h"
#include "Rhombus/Math/Matrix.h"

#include <vector>

namespace rhombus
{
	class SceneGraphNode
	{
	public:
		SceneGraphNode(Scene* scene);
		SceneGraphNode(Entity entity);

		void SetLocalTransform(const Mat4& matrix) { m_localTransform = matrix; }
		const Mat4& GetLocalTransform() const { return m_localTransform; }
		Mat4 GetWorldTransform();

		void SetParent(SceneGraphNode* parentNode) { m_parent = parentNode; }

		Ref<SceneGraphNode> AddChild(Entity entity);
		void AddChild(Ref<SceneGraphNode> sceneGraphNode);
		void RemoveChild(Ref<SceneGraphNode> sceneGraphNode);
		void RemoveAllChildren();
		void MoveChild(Ref<SceneGraphNode> sceneGraphNode, int newOrderIndex);

		SceneGraphNode* GetParent() { return m_parent; }
		const Entity GetEntity() const { return m_entity; }
		bool GetIsRootNode() const { return m_entity.GetContext() == nullptr; }

		const std::vector<Ref<SceneGraphNode>>& GetChildren() { return m_children; }
		std::vector<Ref<SceneGraphNode>>::const_iterator GetChildIteratorStart() { return m_children.begin(); }
		std::vector<Ref<SceneGraphNode>>::const_iterator GetChildIteratorEnd() { return m_children.end(); }

		void SetIsDirty(bool dirty);
		bool GetIsDirty() const { return m_isDirty; }

		void UpdateDirtyTransforms();

	private:
		SceneGraphNode* m_parent;
		Entity m_entity;
		Scene* m_rootScene;
		bool m_isDirty;
		Mat4 m_localTransform;
		Mat4 m_worldTransform;
		std::vector<Ref<SceneGraphNode>> m_children;
	};
}
