#include "EntityViewPanel.h"

#include "Rhombus/Core/Application.h"
#include "Rhombus/ECS/Components/SpriteRendererComponent.h"

#include "Rhombus/ImGui/ImGuiWidgets.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>


namespace rhombus
{
	void EntityViewPanel::OnImGuiRender()
	{
		ImGui::Begin("Entity View");

		if ((EntityID)m_currentEntity != INVALID_ENTITY)
		{
			if (m_currentEntity.HasComponent<SpriteRendererComponent>())
			{
				const SpriteRendererComponent& src = m_currentEntity.GetComponentRead<SpriteRendererComponent>();

				const float height = Application::Get().GetViewport().height * 0.4f;

				ImGui::Sprite(src, height);
			}
		}

		ImGui::End();
	}
}