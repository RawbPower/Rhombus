#pragma once

#include "Rhombus/Scenes/Entity.h"

namespace rhombus
{
	class EntityViewPanel
	{
	public:
		EntityViewPanel() = default;

		void OnImGuiRender();

		void SetCurrentEntity(Entity entity) { m_currentEntity = entity; }

		void ResetCurrentEntity() { m_currentEntity = {}; }

	private:
		Entity m_currentEntity;
	};
}