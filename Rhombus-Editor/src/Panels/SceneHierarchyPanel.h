#pragma once

#include "Rhombus/Core/Core.h"
#include "Rhombus/Core/Log.h"
#include "Rhombus/Scenes/Scene.h"
#include "Rhombus/Scenes/Entity.h"
#include "EditorExtension.h"

namespace rhombus
{
	typedef std::function<void()> AddChildCallback;
	typedef std::function<void(Entity)> SelectEntityCallback;

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);
		void SetEditorExtension(const Ref<EditorExtension>& extension);
		void SetSelectEntityCallback(SelectEntityCallback callback) { m_selectEntityCallback = callback; }

		void OnImGuiRender(bool bIsInEditMode, std::unordered_map<EntityID, bool>& entityEnabledMap);

		Entity GetSelectedEntity() const { return m_selectionContext; }
		void SetSelectedEntity(Entity entity) { m_selectionContext = entity; if (m_selectEntityCallback) m_selectEntityCallback(m_selectionContext); }

		void GetAllSelectedEntities(std::vector<Entity>& selectedEntitiesInOut) const;
		void ResetSelectedEntities() { m_selectionContext = {}; m_selectionMask = 0; }
		std::vector<EntityID> CalculateEntityOrdering() const;

		template<typename T>
		static void DisplayAddComponentEntry(const std::string& entryName, Entity entity);
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
		Ref<EditorExtension> m_editorExtension;
		std::unordered_map<EntityID, float> m_hierarchyEntityPositionMap;
		Entity m_selectionContext;		// Main selected entity (shown in Property View)
		int m_selectionMask = 0;		// All selected entities (can all be dragged at once)
		int m_currentEntityIndex = 0;
		bool m_handledReorderDragAndDrop = false;

		std::queue<Entity> m_destroyQueue;

		// Add child as a callback to be called after all the entities are iterated through
		// This is to avoid invalidating the iterator while going through it
		AddChildCallback m_addChildCallback;
		SelectEntityCallback m_selectEntityCallback;
	};
}
