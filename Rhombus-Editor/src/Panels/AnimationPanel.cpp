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
		float margin = 64.0f;

		static int item_current_idx = 0; // Here we store our selection data as an index.

		// Need to clean up the management here when switching animations to one with a different number of clips
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
			item_current_idx = 0;
		}

		ImGui::SetNextItemWidth(240.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 8));

		std::string combo_preview_value = items[item_current_idx];  // Pass in the preview value visible before opening the combo (it could be anything)
		if (ImGui::BeginCombo("##Animation", combo_preview_value.c_str()))
		{
			for (int n = 0; n < items.size(); n++)
			{
				const bool is_selected = (item_current_idx == n);
				if (ImGui::Selectable(items[n].c_str(), is_selected))
				{
					item_current_idx = n;
					AnimatorComponent& animator = m_currentEntity.GetComponent<AnimatorComponent>();
					animator.Start(animator.GetAnimationClip(n).m_name);
				}

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
		ImTextureID playPauseRendererID = m_isPlaying ? (ImTextureID)m_IconPause->GetRendererID() : (ImTextureID)m_IconPlay->GetRendererID();
		if (ImGui::ImageButton(playPauseRendererID, ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), 0))
		{
			m_isPlaying = !m_isPlaying;
		}

		ImGui::SameLine();

		ImGui::SetCursorPosX((ImGui::GetContentRegionMax().x * 0.5f) - (size * 0.5f) + spacing);
		ImGui::ImageButton((ImTextureID)m_IconForward->GetRendererID(), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), 0);

		ImGui::SameLine();

		ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - margin);
		ImGui::ImageButton((ImTextureID)m_IconLoop->GetRendererID(), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), 0);
		ImGui::SameLine();

		ImGuiStyle& style = ImGui::GetStyle();
		ImGui::SetCursorPosY(ImGui::GetCursorPos().y + 7.0f);

		if ((EntityID)m_currentEntity != INVALID_ENTITY && m_currentEntity.HasComponent<AnimatorComponent>())
		{
			AnimatorComponent& animator = m_currentEntity.GetComponent<AnimatorComponent>();
			AnimationClip& clip = animator.GetAnimationClipRef(item_current_idx);
			ImGui::Checkbox("##Looping", &clip.m_looping);
		}
		else
		{
			bool dummyBool = false;
			ImGui::Checkbox("##Looping", &dummyBool);
		}

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

				// Sprites
				//ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0.0f, 0.0f));
				ImGui::TableNextRow();

				for (int column = 0; column < clip.m_samples.size(); column++)
				{
					//ImGui::TableSetColumnIndex(column+1);
					ImGui::TableSetColumnIndex(column);

					int frameRow = clip.m_samples[column].m_spriteFrame % spriteRenderer.GetColumns();
					int frameCol = spriteRenderer.GetRows() - 1.0f - clip.m_samples[column].m_spriteFrame / spriteRenderer.GetColumns();
					Ref<SubTexture2D> sampleFrame = SubTexture2D::CreateFromCoords(spriteRenderer.m_texture, Vec2(frameRow, frameCol), spriteRenderer.GetSpriteSize(), spriteRenderer.GetPadding());
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

					ImGui::PushID(column);
					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, iconPadding);
					if (ImGui::ImageButton((ImTextureID)spriteRenderer.m_texture->GetRendererID(), iconSize, uv0, uv1))
					{
						m_currentSampleIndex = column;
					}
					ImGui::PopStyleVar();
					ImGui::PopID();
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

				//ImGui::PopStyleVar();

				// Time steps
				//ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0.0f, defaultCellPadding.y));

				ImGui::TableNextRow();

				for (int column = 0; column < clip.m_samples.size(); column++)
				{
					ImGui::TableSetColumnIndex(column);

					char frameTimeLabel[32];
					sprintf(frameTimeLabel, "%.02fs", clip.m_samples[column].m_duration);
					ImGui::SetCursorPosX(ImGui::GetCursorPos().x + thumbnailSize * 0.5f + thumbnailPadding - ImGui::CalcTextSize(frameTimeLabel).x * 0.5f);
					ImGui::Text(frameTimeLabel);
				}

				ImGui::TableSetColumnIndex(clip.m_samples.size());
				ImGui::SetCursorPosX(ImGui::GetCursorPos().x + thumbnailSize * 0.5f + thumbnailPadding - ImGui::CalcTextSize("Add").x * 0.5f);
				ImGui::Text("Add");

				//ImGui::PopStyleVar();

				ImGui::EndTable();
			}
		}

		ImGui::PopStyleColor();

		ImGui::End();

		if (m_currentSampleIndex >= 0 && (EntityID)m_currentEntity != INVALID_ENTITY && m_currentEntity.HasComponent<AnimatorComponent>())
		{
			ImGui::Begin("Sample Properties");

			AnimatorComponent& animator = m_currentEntity.GetComponent<AnimatorComponent>();
			const SpriteRendererComponent& spriteRenderer = m_currentEntity.GetComponentRead<SpriteRendererComponent>();
			AnimationClip clip = animator.GetAnimationClip(item_current_idx);

			float duration = clip.m_samples[m_currentSampleIndex].m_duration;
			int frame = clip.m_samples[m_currentSampleIndex].m_spriteFrame;

			if (spriteRenderer.m_texture)
			{
				const float thumbnailSize = 96.0f;
				const float thumbnailPadding = 8.0f;

				int frameRow = clip.m_samples[m_currentSampleIndex].m_spriteFrame % spriteRenderer.GetColumns();
				int frameCol = spriteRenderer.GetRows() - 1.0f - clip.m_samples[m_currentSampleIndex].m_spriteFrame / spriteRenderer.GetColumns();
				Ref<SubTexture2D> sampleFrame = SubTexture2D::CreateFromCoords(spriteRenderer.m_texture, Vec2(frameRow, frameCol), spriteRenderer.GetSpriteSize(), spriteRenderer.GetPadding());
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
				ImGui::Image((ImTextureID)spriteRenderer.m_texture->GetRendererID(), iconSize, uv0, uv1);
				ImGui::PopStyleVar();

				ImGui::Spacing();
			}
			else
			{
				ImGui::Button("Drag Texture", ImVec2(100.0f, 0.0f));
			}

			ImGui::Spacing();

			if (ImGui::InputFloat("Duration", &duration) && duration >= 0.0f)
			{
				clip.m_samples[m_currentSampleIndex].m_duration = duration;
			}

			if (ImGui::InputInt("Frame", &frame) && frame >= 0)
			{
				clip.m_samples[m_currentSampleIndex].m_spriteFrame = frame;
			}

			ImGui::End();
		}
	}
}