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

		void OnImGuiRender(bool bIsInEditMode, std::unordered_map<EntityID, bool>& entityEnabledMap);

		void MoveEntityInHierarchyOrder(EntityID entityID, int newOrderIndex);
		void ReloadHieararchyEntities(std::unordered_map<EntityID, bool>& entityEnabledMap);

		Entity GetSelectedEntity() const { return m_selectionContext; }
		void SetSelectedEntity(Entity entity) { m_selectionContext = entity; }

		void GetAllSelectedEntities(std::vector<Entity>& selectedEntitiesInOut) const;
		void ResetSelectedEntities() { m_selectionContext = {}; m_selectionMask = 0; }
		std::vector<EntityID> CalculateEntityOrdering() const;

		void SetHierarchyDirty() { m_hierarchyDirtyFlag = true; }
	private:
		struct HierarchyEntity
		{
			EntityID m_entityID;
			float m_yPos;

			HierarchyEntity(EntityID entityID)
				: m_entityID(entityID)
			{

			}
		};

		template<typename T>
		void DisplayAddComponentEntry(const std::string& entryName);

		void DrawEntityNode(Entity entity, int i);
		void DrawComponents(Entity entity);

		bool IsEntityInHierarchy(EntityID entityID) const;

		Ref<Scene> m_context;
		std::vector<HierarchyEntity> m_hierarchyEntityOrder;
		//std::unordered_map<EntityID, bool> m_entityEnabledMap;
		Entity m_selectionContext;		// Main selected entity (shown in Property View)
		int m_selectionMask;			// All selected entities (can all be dragged at once)
		bool m_handledReorderDragAndDrop;
		bool m_hierarchyDirtyFlag;
	};
}
