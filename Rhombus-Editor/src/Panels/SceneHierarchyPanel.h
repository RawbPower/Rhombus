#pragma once

#include "Rhombus/Core/Core.h"
#include "Rhombus/Core/Log.h"
#include "Rhombus/Scenes/Scene.h"
#include "Rhombus/Scenes/Entity.h"

namespace rhombus
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);

		void OnImGuiRender(bool bIsInEditMode, std::unordered_map<EntityID, bool>& entityEnabledMap);;

		Entity GetSelectedEntity() const { return m_selectionContext; }
		void SetSelectedEntity(Entity entity) { m_selectionContext = entity; }

		void GetAllSelectedEntities(std::vector<Entity>& selectedEntitiesInOut) const;
		void ResetSelectedEntities() { m_selectionContext = {}; m_selectionMask = 0; }
		std::vector<EntityID> CalculateEntityOrdering() const;
	private:

		template<typename T>
		void DisplayAddComponentEntry(const std::string& entryName);

		void DrawEntityNode(Entity entity, Ref<SceneGraphNode> parentNode, bool bIsInEditMode, std::unordered_map<EntityID, bool>& entityEnabledMap);
		void DrawComponents(Entity entity);

		void CreateEntityHierarchyTable(const char* label, const Ref<SceneGraphNode> parentNode, bool bIsInEditMode, std::unordered_map<EntityID, bool>& entityEnabledMap);
		int FindOrderingAndParentFromMousePosition(float cursorY, EntityID movingEntity, Ref<SceneGraphNode>& parentNodeInOut);

		static void MoveEntityInHierarchyOrder(Entity entity, Ref<SceneGraphNode>& parentNode, int newOrderIndex);
		static void SetEntityAsChild(Entity entity, Ref<SceneGraphNode>& parentNode);

	private:
		Ref<Scene> m_context;
		std::unordered_map<EntityID, float> m_hierarchyEntityPositionMap;
		Entity m_selectionContext;		// Main selected entity (shown in Property View)
		int m_selectionMask;			// All selected entities (can all be dragged at once)
		int m_currentEntityIndex;
		bool m_handledReorderDragAndDrop;

		// Add child as a callback to be called after all the entities are iterated through
		// This is to avoid invalidating the iterator while going through it
		std::function<void()> m_addChildCallback;
	};
}
