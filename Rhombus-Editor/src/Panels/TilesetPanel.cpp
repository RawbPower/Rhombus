#include "rbpch.h"

#include "TilesetPanel.h"

#include "Rhombus/Core/Application.h"
#include "Rhombus/Project/Project.h"

#include <imgui/imgui.h>

namespace rhombus
{
	TilesetPanel::TilesetPanel()
	{
		m_Tileset = Texture2D::Create("assets/sprites/VolleyTiles.png");
		
		m_iRowCount = 3;
		m_iColumnCount = 4;
		m_iSelectedTileIndex = -1;

		SubTexture2D::SliceTexture(m_Tileset, m_iRowCount, m_iColumnCount, 1, m_Tiles);
	}

	void TilesetPanel::OnImGuiRender()
	{
		ImGui::Begin("Tileset");

		float thumbnailSize = 48.0f;

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

		static ImGuiTableFlags flags = ImGuiTableFlags_NoPadInnerX | ImGuiTableFlags_NoPadOuterX;
		if (ImGui::BeginTable("Tileset", m_iColumnCount, flags))
		{
			ImGuiStyle& style = ImGui::GetStyle();
			auto& colors = style.Colors;
			for (int row = 0; row < m_iRowCount; row++)
			{
				ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(style.CellPadding.x, 0.0f));
				ImGui::TableNextRow();
				for (int column = 0; column < m_iColumnCount; column++)
				{
					ImGui::TableSetColumnIndex(column);
					int tileIndex = column + row * m_iColumnCount;
					ImGui::PushID(tileIndex);
					const Ref<SubTexture2D>& tileTexture = m_Tiles[tileIndex];

					// Whe tile is selected, switch the button color to active
					if (tileIndex == m_iSelectedTileIndex)
					{
						ImGui::PopStyleColor();
						ImGui::PushStyleColor(ImGuiCol_Button, style.Colors[ImGuiCol_ButtonActive]);
					}

					if (ImGui::ImageButton((ImTextureID)tileTexture->GetTexture()->GetRendererID(), { thumbnailSize, thumbnailSize }, ImVec2(tileTexture->GetTexCoords()[3].x, tileTexture->GetTexCoords()[3].y), ImVec2(tileTexture->GetTexCoords()[1].x, tileTexture->GetTexCoords()[1].y)))
					{
						if (tileIndex != m_iSelectedTileIndex)
						{
							m_iSelectedTileIndex = tileIndex;
						}
						else
						{
							m_iSelectedTileIndex = -1;
						}
					}
					
					// Don't forget to switch the color back
					if (tileIndex == m_iSelectedTileIndex)
					{
						ImGui::PopStyleColor();
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
					}
					ImGui::PopID();
				}
				ImGui::PopStyleVar();
			}
			ImGui::EndTable();
		}

		ImGui::PopStyleColor();

		ImGui::End();
	}
}