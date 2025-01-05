#include "rbpch.h"

#include "AnimationPanel.h"

#include "Rhombus/Core/Application.h"
#include "Rhombus/ECS/Components/AnimatorComponent.h"
#include "Rhombus/ECS/Components/SpriteRendererComponent.h"

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

		std::vector<std::string> items;
		if ((EntityID)m_currentEntity != INVALID_ENTITY && m_currentEntity.HasComponent<AnimatorComponent>())
		{
			const AnimatorComponent& animator = m_currentEntity.GetComponentRead<AnimatorComponent>();
			for (int i = 0; i < animator.GetAnimationCount(); i++)
			{
				items.push_back(animator.GetAnimationClip(i).m_name);
			}
		}
		else
		{
			items.push_back("Default");
		}

		static int item_current_idx = 0; // Here we store our selection data as an index.
		ImGui::SetNextItemWidth(240.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 8));

		std::string combo_preview_value = items[item_current_idx];  // Pass in the preview value visible before opening the combo (it could be anything)
		if (ImGui::BeginCombo("##Animation", combo_preview_value.c_str()))
		{
			for (int n = 0; n < items.size(); n++)
			{
				const bool is_selected = (item_current_idx == n);
				if (ImGui::Selectable(items[n].c_str(), is_selected))
					item_current_idx = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

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

		ImGui::Spacing();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

		if ((EntityID)m_currentEntity != INVALID_ENTITY && m_currentEntity.HasComponent<AnimatorComponent>())
		{
			const float thumbnailSize = 64.0f;
			const float thumbnailPadding = 8.0f;

			const AnimatorComponent& animator = m_currentEntity.GetComponentRead<AnimatorComponent>();
			const SpriteRendererComponent& spriteRenderer = m_currentEntity.GetComponentRead<SpriteRendererComponent>();
			const AnimationClip clip = animator.GetAnimationClip(item_current_idx);
			static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoHostExtendX;
			if (ImGui::BeginTable("Timeline", clip.m_samples.size() + 1, flags))
			{
				for (int i = 0; i < clip.m_samples.size()+1; i++)
				{
					char buf[32];
					sprintf(buf, "Frame %d", i+1);
					ImGui::TableSetupColumn(buf);
				}
				ImGui::TableHeadersRow();

				for (int row = 0; row < 1; row++)
				{
					ImGui::TableNextRow();
					/*ImGui::TableSetColumnIndex(0);
					
					const char* title = "Sprite";
					ImGui::SetCursorPosY(ImGui::GetCursorPos().y + (thumbnailSize + thumbnailPadding) * 0.5f - ImGui::CalcTextSize(title).y * 0.5f);

					ImGui::Text(title);*/

					for (int column = 0; column < clip.m_samples.size(); column++)
					{
						//ImGui::TableSetColumnIndex(column+1);
						ImGui::TableSetColumnIndex(column);

						Ref<SubTexture2D> sampleFrame = SubTexture2D::CreateFromCoords(spriteRenderer.m_texture, Vec2(clip.m_samples[column].m_spriteFrame % spriteRenderer.GetColumns(), spriteRenderer.GetRows() - 1.0f - spriteRenderer.GetFrame() / spriteRenderer.GetRows()), spriteRenderer.GetSpriteSize(), spriteRenderer.GetPadding());
						ImVec2 uv0 = ImVec2(sampleFrame->GetTexCoords()[3].x, sampleFrame->GetTexCoords()[3].y);
						ImVec2 uv1 = ImVec2(sampleFrame->GetTexCoords()[1].x, sampleFrame->GetTexCoords()[1].y);
						float textureWidth = (float)sampleFrame->GetWidth();
						float textureHeight = (float)sampleFrame->GetHeight();

						// Caclulate image size
						const float fAspectRatio = textureWidth / textureHeight;
						const bool bWideImage = fAspectRatio >= 1.0f;
						const ImVec2 iconSize = bWideImage ? ImVec2(thumbnailSize, thumbnailSize * (1.0f / fAspectRatio)) : ImVec2(thumbnailSize * fAspectRatio, thumbnailSize);

						// Caclulate image padding needed to make square button
						const float fDimensionDifference = math::Abs(textureWidth - textureHeight);
						const float fMaxDimension = bWideImage ? textureWidth : textureHeight;
						const float fSmallDimensionPadding = (fDimensionDifference / fMaxDimension) * thumbnailSize * 0.5f;
						const ImVec2 iconPadding = bWideImage ? ImVec2(thumbnailPadding, fSmallDimensionPadding + thumbnailPadding) : ImVec2(fSmallDimensionPadding + thumbnailPadding, thumbnailPadding);

						ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, iconPadding);
						ImGui::ImageButton((ImTextureID)spriteRenderer.m_texture->GetRendererID(), iconSize, uv0, uv1);
						ImGui::PopStyleVar();
					}

					ImGui::TableSetColumnIndex(clip.m_samples.size());
					// Caclulate image size
					const float fAspectRatio = (float)m_IconAdd->GetWidth() / (float)m_IconAdd->GetHeight();
					const bool bWideImage = fAspectRatio >= 1.0f;
					const ImVec2 iconSize = bWideImage ? ImVec2(thumbnailSize, thumbnailSize * (1.0f / fAspectRatio)) : ImVec2(thumbnailSize * fAspectRatio, thumbnailSize);

					// Caclulate image padding needed to make square button
					const float fDimensionDifference = math::Abs(m_IconAdd->GetWidth() - m_IconAdd->GetHeight());
					const float fMaxDimension = bWideImage ? m_IconAdd->GetWidth() : m_IconAdd->GetHeight();
					const float fSmallDimensionPadding = (fDimensionDifference / fMaxDimension) * thumbnailSize * 0.5f;
					const ImVec2 iconPadding = bWideImage ? ImVec2(thumbnailPadding, fSmallDimensionPadding + thumbnailPadding) : ImVec2(fSmallDimensionPadding + thumbnailPadding, thumbnailPadding);

					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, iconPadding);
					ImGui::ImageButton((ImTextureID)m_IconAdd->GetRendererID(), iconSize, { 0,1 }, { 1,0 });
					ImGui::PopStyleVar();
				}
				ImGui::EndTable();
			}
		}

		ImGui::PopStyleColor();

		ImGui::End();
	}
}