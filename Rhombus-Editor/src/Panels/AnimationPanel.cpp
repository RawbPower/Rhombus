#include "rbpch.h"

#include "AnimationPanel.h"

#include "Rhombus/Core/Application.h"

#include <imgui/imgui.h>

namespace rhombus
{
	AnimationPanel::AnimationPanel()
	{
		m_IconPlay = Texture2D::Create(Application::Get().GetPathRelativeToEditorDirectory("resources/icons/AnimationPlay.png"));
		m_IconPause = Texture2D::Create(Application::Get().GetPathRelativeToEditorDirectory("resources/icons/AnimationPause.png"));
		m_IconForward = Texture2D::Create(Application::Get().GetPathRelativeToEditorDirectory("resources/icons/AnimationForward.png"));
		m_IconBackward = Texture2D::Create(Application::Get().GetPathRelativeToEditorDirectory("resources/icons/AnimationBackward.png"));
		m_IconLoop = Texture2D::Create(Application::Get().GetPathRelativeToEditorDirectory("resources/icons/AnimationLoop.png"));
		m_IconAdd = Texture2D::Create(Application::Get().GetPathRelativeToEditorDirectory("resources/icons/AnimationAdd.png"));
		m_IconRename = Texture2D::Create(Application::Get().GetPathRelativeToEditorDirectory("resources/icons/AnimationRename.png"));
		m_IconDelete = Texture2D::Create(Application::Get().GetPathRelativeToEditorDirectory("resources/icons/AnimationDelete.png"));
	}

	void AnimationPanel::OnImGuiRender()
	{
		ImGui::Begin("Animation");

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));

		float size = 32.0f;
		float spacing = 48.0f;
		float margin = 48.0f;

		const char* items[] = { "Idle", "Run", "Jump", "WallSlide" };
		static int item_current_idx = 0; // Here we store our selection data as an index.
		ImGui::SetNextItemWidth(240.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 8));
		ImGui::Combo("##Animation", &item_current_idx, items, IM_ARRAYSIZE(items));
		ImGui::PopStyleVar();

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		ImGui::ImageButton((ImTextureID)m_IconAdd->GetRendererID(), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), 0);

		ImGui::SameLine();

		ImGui::ImageButton((ImTextureID)m_IconRename->GetRendererID(), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), 0);

		ImGui::SameLine();

		ImGui::ImageButton((ImTextureID)m_IconDelete->GetRendererID(), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), 0);

		ImGui::SameLine();

		ImGui::SetCursorPosX((ImGui::GetContentRegionMax().x * 0.5f) - (size * 0.5f) - spacing);
		ImGui::ImageButton((ImTextureID)m_IconBackward->GetRendererID(), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), 0);

		ImGui::SameLine();

		ImGui::SetCursorPosX((ImGui::GetContentRegionMax().x * 0.5f) - (size * 0.5f));
		ImGui::ImageButton((ImTextureID)m_IconPlay->GetRendererID(), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), 0);

		ImGui::SameLine();

		ImGui::SetCursorPosX((ImGui::GetContentRegionMax().x * 0.5f) - (size * 0.5f) + spacing);
		ImGui::ImageButton((ImTextureID)m_IconForward->GetRendererID(), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), 0);

		ImGui::SameLine();

		ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - margin);
		ImGui::ImageButton((ImTextureID)m_IconLoop->GetRendererID(), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), 0);

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);

		ImGui::Separator();

		ImGui::End();
	}
}