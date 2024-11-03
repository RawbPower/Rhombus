#pragma once
#include "Rhombus/Core/Core.h"
#include "Rhombus/Scenes/Entity.h"

#include "Rhombus/ImGui/ImGuiWidgets.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace rhombus
{
	class EditorExtension
	{
	public:
		//EditorExtension();
		//~EditorExtension() = default;

		virtual void DisplayAddComponentList(Entity entity) {};
		virtual void DisplayComponentProperties(Entity entity) {};

		template<typename T>
		static void DisplayAddComponentEntry(const std::string& entryName, Entity entity)
		{
			if (!entity.HasComponent<T>())
			{
				if (ImGui::MenuItem(entryName.c_str()))
				{
					entity.AddComponent<T>();
					ImGui::CloseCurrentPopup();
				}
			}
		}

		template<typename T, typename UIFunction>
		void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
		{
			if (entity.HasComponent<T>())
			{
				auto& component = entity.GetComponent<T>();

				const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;
				ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 6 });		// Note: Look into how this function works more
				float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
				//ImGui::Separator();
				bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
				ImGui::PopStyleVar();

				ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
				if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
				{
					ImGui::OpenPopup("ComponentSettings");
				}

				bool removeComponent = false;
				if (ImGui::BeginPopup("ComponentSettings"))
				{
					if (ImGui::MenuItem("Remove Component"))
						removeComponent = true;

					ImGui::EndPopup();
				}

				if (open)
				{
					ImGuiStyle& style = ImGui::GetStyle();
					ImGui::Indent(-0.8f * style.IndentSpacing);
					uiFunction(component);
					ImGui::Indent(0.8f * style.IndentSpacing);
					ImGui::TreePop();
				}

				if (removeComponent)
					entity.RemoveComponent<T>();
			}
		}

	private:

	};
}

