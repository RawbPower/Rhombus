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

		void OnImGuiRender();

		Entity GetSelectedEntity() const { return m_selectionContext; }
		void SetSelectedEntity(Entity entity) { m_selectionContext = entity; }
	private:
		template<typename T>
		void DisplayAddComponentEntry(const std::string& entryName);

		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

		Ref<Scene> m_context;
		Entity m_selectionContext;
	};
}
