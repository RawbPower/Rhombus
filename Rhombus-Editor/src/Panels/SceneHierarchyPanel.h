#pragma once

#include "Rhombus/Core/Core.h"
#include "Rhombus/Core/Log.h"
#include "Rhombus/ECS/Scene.h"
#include "Rhombus/ECS/Entity.h"

namespace rhombus
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);

		void OnImGuiRender(bool bIsInEditMode, std::unordered_map<EntityID, bool>& entityEnabledMap);

		void ReloadHieararchyEntities(std::unordered_map<EntityID, bool>& entityEnabledMap);

		Entity GetSelectedEntity() const { return m_selectionContext; }
		void SetSelectedEntity(Entity entity) { m_selectionContext = entity; }

		void GetAllSelectedEntities(std::vector<Entity>& selectedEntitiesInOut) const;
		//const std::unordered_map<EntityID, bool>& GetEntityEnabledMap() const { return m_entityEnabledMap; }
	private:
		template<typename T>
		void DisplayAddComponentEntry(const std::string& entryName);

		void DrawEntityNode(Entity entity, int i);
		void DrawComponents(Entity entity);

		Ref<Scene> m_context;
		std::vector<EntityID> m_hierarchyEntityOrder;
		//std::unordered_map<EntityID, bool> m_entityEnabledMap;
		Entity m_selectionContext;		// Main selected entity (shown in Property View)
		int m_selectionMask;			// All selected entities (can all be dragged at once)
	};
}
