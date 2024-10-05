#include "SceneHierarchyPanel.h"

#include "Rhombus/Scripting/ScriptEngine.h"
#include "Rhombus/ECS/ECSTypes.h"
#include "Components/CardComponent.h"
#include "Components/CardSlotComponent.h"
#include "Components/PatienceComponent.h"

#include "Rhombus/ImGui/ImGuiWidgets.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <filesystem>

namespace rhombus
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetHierarchyDirty();
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_context = context;
		m_selectionContext = {};
		m_selectionMask = 0;
	}

	void SceneHierarchyPanel::OnImGuiRender(bool bIsInEditMode, std::unordered_map<EntityID, bool>& entityEnabledMap)
	{
		m_handledReorderDragAndDrop = false;
		ReloadHieararchyEntities(entityEnabledMap);

		ImGui::Begin("Scene Hierarchy");

		if (ImGui::BeginTable("Scene Hierarchy Table", 2))
		{
			ImGuiStyle& style = ImGui::GetStyle();
			ImGui::TableSetupColumn("##Entities", ImGuiTableColumnFlags_WidthFixed, ImGui::GetContentRegionAvail().x - style.IndentSpacing);
			ImGui::TableSetupColumn("##Active", ImGuiTableColumnFlags_WidthFixed); 
			int i = 0;
			ImGui::TableNextRow();
			for (HierarchyEntity& hierarchyEntity : m_hierarchyEntityOrder)
			{
				ImGui::TableSetColumnIndex(0);
				Entity entity(hierarchyEntity.m_entityID, m_context.get());
				DrawEntityNode(entity, i++);
				hierarchyEntity.m_yPos = ImGui::GetItemRectMin().y;

				if (bIsInEditMode)
				{
					ImGui::TableSetColumnIndex(1);

					ImGuiStyle& style = ImGui::GetStyle();
					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, (float)(int)(style.FramePadding.y * 0.3f)));

					ImGui::PushID(i);
					ImGui::Checkbox("##enabled", &entityEnabledMap[hierarchyEntity.m_entityID]);

					ImGui::PopID();

					ImGui::PopStyleVar(1);
				}

				ImGui::TableNextRow();
			}
			ImGui::EndTable();
		}

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			m_selectionContext = {};

			if (!ImGui::GetIO().KeyCtrl)
			{
				m_selectionMask = 0;
			}
		}

		// Right-click on blank space
		if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem("Create New Entity"))
			{
				m_context->CreateEntity("Untitled Entity");
				SetHierarchyDirty();
			}

			ImGui::EndPopup();
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_HIERACHY_UUID"))
			{
				if (!m_handledReorderDragAndDrop)
				{
					float cursorY = ImGui::GetMousePos().y;
					int desiredOrdering = 0;
					EntityID droppedID = m_context->GetEntityByUUID(*(UUID*)payload->Data);
					for (HierarchyEntity hierarchyEntity : m_hierarchyEntityOrder)
					{
						// Find the new position for the entity by comparing to each entity tree position in the hierarchy
						if (cursorY < hierarchyEntity.m_yPos)
						{
							break;
						}
						desiredOrdering++;
					}
					MoveEntityInHierarchyOrder(droppedID, desiredOrdering);
					SetHierarchyDirty();
					m_handledReorderDragAndDrop = true;
				}
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::End();

		ImGui::Begin("Properties");

		if (m_selectionContext)
		{
			DrawComponents(m_selectionContext);
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::MoveEntityInHierarchyOrder(EntityID entityID, int newOrderIndex)
	{
		int currentOrderIndex = 0;
		for (HierarchyEntity hierarchyEntity : m_hierarchyEntityOrder)
		{
			if (hierarchyEntity.m_entityID == entityID)
			{
				break;
			}
			currentOrderIndex++;
		}

		if (currentOrderIndex > newOrderIndex)
			std::rotate(m_hierarchyEntityOrder.rend() - currentOrderIndex - 1, m_hierarchyEntityOrder.rend() - currentOrderIndex, m_hierarchyEntityOrder.rend() - newOrderIndex);
		else
			std::rotate(m_hierarchyEntityOrder.begin() + currentOrderIndex, m_hierarchyEntityOrder.begin() + currentOrderIndex + 1, m_hierarchyEntityOrder.begin() + newOrderIndex + 1);
	}

	bool SceneHierarchyPanel::IsEntityInHierarchy(EntityID entityID) const
	{
		return std::find_if(m_hierarchyEntityOrder.begin(), m_hierarchyEntityOrder.end(), [&](const auto& val) { return val.m_entityID == entityID; }) != m_hierarchyEntityOrder.end();
	}

	void SceneHierarchyPanel::ReloadHieararchyEntities(std::unordered_map<EntityID, bool>& entityEnabledMap)
	{
		if (m_hierarchyDirtyFlag)
		{
			std::vector<EntityID> allEntities = m_context->GetAllEntities();

			std::vector<HierarchyEntity>::iterator iter;
			for (iter = m_hierarchyEntityOrder.begin(); iter != m_hierarchyEntityOrder.end(); )
			{
				if (std::find(allEntities.begin(), allEntities.end(), iter->m_entityID) == allEntities.end())
				{
					entityEnabledMap.erase(iter->m_entityID);
					iter = m_hierarchyEntityOrder.erase(iter);
				}
				else
				{
					++iter;
				}
			}

			for (EntityID entityID : allEntities)
			{
				if (!IsEntityInHierarchy(entityID))
				{
					m_hierarchyEntityOrder.push_back(entityID);
					if (entityEnabledMap.find(entityID) == entityEnabledMap.end())
					{
						entityEnabledMap[entityID] = true;
					}
				}
			}
			m_hierarchyDirtyFlag = false;
		}
	}

	void SceneHierarchyPanel::GetAllSelectedEntities(std::vector<Entity>& selectedEntitiesInOut) const
	{
		std::vector<EntityID> entities = m_context->GetAllEntities();
		int i = 0;
		for (EntityID entityID : entities)
		{
			if (m_selectionMask & (1 << i))
			{
				Entity entity(entityID, m_context.get());
				selectedEntitiesInOut.push_back(entity);
			}
		}
	}

	std::vector<EntityID> SceneHierarchyPanel::CalculateEntityOrdering() const
	{
		std::vector<EntityID> entityOrdering;

		for (HierarchyEntity hierarchyEntity : m_hierarchyEntityOrder)
		{
			entityOrdering.push_back(hierarchyEntity.m_entityID);
		}

		return entityOrdering;
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity, int i)
	{
		auto& tag = entity.GetComponent<TagComponent>().m_tag;
		
		ImGuiTreeNodeFlags flags = ((m_selectionMask & (1 << i)) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		if (ImGui::IsItemReleased())
		{
			if (ImGui::GetIO().KeyCtrl)
			{
				m_selectionMask ^= (1 << i);
			}
			else
			{
				m_selectionMask = (1 << i);
			}
			m_selectionContext = entity;
		}

		if (ImGui::BeginDragDropSource())
		{
			UUID* uuid = &entity.GetUUID();
			ImGui::SetDragDropPayload("SCENE_HIERACHY_UUID", uuid, sizeof(UUID));
			ImGui::Text(entity.GetName().c_str());
			ImGui::EndDragDropSource();
		}
		
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_HIERACHY_UUID"))
			{
				// TODO: Make the entity a child of this one
				m_handledReorderDragAndDrop = true;
			}
			ImGui::EndDragDropTarget();
		}

		bool entityDeleted = false;
		bool entityDuplicated = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Duplicate"))
				entityDuplicated = true;

			if (ImGui::MenuItem("Delete"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			bool childOpened = ImGui::TreeNodeEx((void*)((uint64_t)(uint32_t)(entity) + 1000), flags, "Child");
			if (childOpened)
				ImGui::TreePop();
			ImGui::TreePop();
		}

		// Duplicate entity
		if (entityDuplicated)
		{
			m_context->DuplicateEntity(entity);
			SetHierarchyDirty();
		}

		// Deferred deletion
		if (entityDeleted)
		{
			m_context->DestroyEntity(entity);
			SetHierarchyDirty();
			if (m_selectionContext == entity)
			{
				m_selectionContext = {};
			}
		}
	}

	static void DrawVec3Control(const std::string& label, Vec3& values, float resetValue = 0.0f, float columnWidth = 70.0f)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0,0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImVec4 pastelRed{ 0.976f, 0.5098f, 0.5176f, 1.0f };
		ImVec4 pastelGreen{ 0.5294f, 0.6588f, 0.5373f, 1.0f };
		ImVec4 pastelBlue{ 0.5345f, 0.7f, 0.854f, 1.0f };
		ImGui::PushStyleColor(ImGuiCol_Button, pastelRed);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, pastelRed);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, pastelRed);
		ImGui::Button("X", buttonSize);
		//if (ImGui::Button("X", buttonSize))
		//	values.x = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine(); 

		ImGui::PushStyleColor(ImGuiCol_Button, pastelGreen);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, pastelGreen);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, pastelGreen);
		ImGui::Button("Y", buttonSize);
		//if (ImGui::Button("Y", buttonSize))
		//	values.y = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, pastelBlue);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, pastelBlue);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, pastelBlue);
		ImGui::Button("Z", buttonSize);
		//if (ImGui::Button("Z", buttonSize))
		//	values.z = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
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
				ImGui::Indent(-0.8f*style.IndentSpacing);
				uiFunction(component);
				ImGui::Indent(0.8f*style.IndentSpacing);
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().m_tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			// Engine
			DisplayAddComponentEntry<CameraComponent>("Camera");
			DisplayAddComponentEntry<ScriptComponent>("Script");
			DisplayAddComponentEntry<SpriteRendererComponent>("Sprite Renderer");
			DisplayAddComponentEntry<CircleRendererComponent>("Circle Renderer");
			DisplayAddComponentEntry<Rigidbody2DComponent>("Rigidbody 2D");
			DisplayAddComponentEntry<BoxCollider2DComponent>("Box Collider 2D");
			DisplayAddComponentEntry<CircleCollider2DComponent>("Circle Collider 2D");
			DisplayAddComponentEntry<BoxArea2DComponent>("Box Area 2D");

			// Game
			DisplayAddComponentEntry<CardComponent>("Card");
			DisplayAddComponentEntry<CardSlotComponent>("Card Slot");
			DisplayAddComponentEntry<PatienceComponent>("Patience");

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
		{
			DrawVec3Control("Position", component.m_position);
			Vec3 rotation = component.m_rotation * math::RadToDeg;
			DrawVec3Control("Rotation", rotation);
			component.m_rotation = rotation * math::DegToRad;
			DrawVec3Control("Scale", component.m_scale, 1.0f);
		});

		DrawComponent<ScriptComponent>("Script", entity, [](auto& component)
		{
			static char buffer[64];
			strcpy(buffer, component.m_scriptName.c_str());

			if (ImGui::InputText("Script", buffer, sizeof(buffer)))
				component.m_scriptName = buffer;
		});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component)
		{
			ImGui::ColorEdit4("Colour", component.GetColor().ToPtr());

			ImGui::Button("Texture", ImVec2(100.0f, 0.0f));
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::filesystem::path texturePath(path);
					Ref<Texture2D> texture = Texture2D::Create(texturePath.string());
					if (texture->IsLoaded())
						component.m_texture = texture;
					else
						RB_WARN("Could not load texture {0}", texturePath.filename().string());
				}
				ImGui::EndDragDropTarget();
			}
		});

		DrawComponent<CircleRendererComponent>("Circle Renderer", entity, [](auto& component)
		{
			ImGui::ColorEdit4("Color", component.m_color.ToPtr());
			ImGui::DragFloat("Thickness", &component.m_thickness, 0.025f, 0.0f, 1.0f);
			ImGui::DragFloat("Fade", &component.m_fade, 0.00025f, 0.0f, 1.0f);
		});

		DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
		{
			auto& camera = component.GetCamera();

			ImGui::Checkbox("Primary", &component.GetIsPrimaryCamera());

			const char* projectionTypeStrings[]{ "Orthographic", "Perspective" };
			const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
			if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
			{
				for (int i = 0; i < 2; i++)
				{
					bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
					if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
					{
						currentProjectionTypeString = projectionTypeStrings[i];
						camera.SetProjectionType((SceneCamera::ProjectionType)i);
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
			{
				float orthoSize = camera.GetOrthographicSize();
				if (ImGui::DragFloat("Size", &orthoSize))
					camera.SetOrthographicSize(orthoSize);

				float orthoNear = camera.GetOrthographicNearClip();
				if (ImGui::DragFloat("Near Clip", &orthoNear))
					camera.SetOrthographicNearClip(orthoNear);

				float orthoFar = camera.GetOrthographicFarClip();
				if (ImGui::DragFloat("Far Clip", &orthoFar))
					camera.SetOrthographicFarClip(orthoFar);

				bool pixelPerfect = camera.GetPixelPerfect();
				if (ImGui::Checkbox("Pixel Perfect", &pixelPerfect))
					camera.SetPixelPerfect(pixelPerfect);

				ImGui::Checkbox("Fixed Aspect Ratio", &component.GetHasFixedAspectRatio());
			}

			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
			{
				float persFOV = camera.GetPerspectiveVerticalFOV() * math::RadToDeg;
				if (ImGui::DragFloat("Vertical FOV", &persFOV))
					camera.SetPerspectiveVerticalFOV(persFOV * math::DegToRad);

				float persNear = camera.GetPerspectiveNearClip();
				if (ImGui::DragFloat("Near Clip", &persNear))
					camera.SetPerspectiveNearClip(persNear);

				float persFar = camera.GetPerspectiveFarClip();
				if (ImGui::DragFloat("Far Clip", &persFar))
					camera.SetPerspectiveFarClip(persFar);
			}
		});

		DrawComponent<Rigidbody2DComponent>("Rigidbody 2D", entity, [](auto& component)
		{
			const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };
			ImGui::SelectableEnum("Body Type", bodyTypeStrings, 3, &((int)component.m_type));

			ImGui::Checkbox("Fixed Rotation", &component.m_fixedRotation);
		});

		DrawComponent<BoxCollider2DComponent>("Box Collider 2D", entity, [](auto& component)
		{
			ImGui::DragFloat2("Offset", component.m_offset.ToPtr(), 0.01f);
			ImGui::DragFloat2("Size", component.m_size.ToPtr());
			ImGui::DragFloat("Density", &component.m_density, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Friction", &component.m_friction, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution", &component.m_restitution, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution Threshold", &component.m_restitutionThreshold, 0.01f, 0.0f);
		});

		DrawComponent<CircleCollider2DComponent>("Circle Collider 2D", entity, [](auto& component)
		{
			ImGui::DragFloat2("Offset", component.m_offset.ToPtr(), 0.01f);
			ImGui::InputFloat("Radius", &component.m_radius);
			ImGui::DragFloat("Density", &component.m_density, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Friction", &component.m_friction, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution", &component.m_restitution, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution Threshold", &component.m_restitutionThreshold, 0.01f, 0.0f);
		});

		DrawComponent<BoxArea2DComponent>("Box Area 2D", entity, [](auto& component)
		{
			ImGui::DragFloat2("Offset", component.m_offset.ToPtr(), 0.01f);
			ImGui::DragFloat2("Size", component.m_size.ToPtr());
			ImGui::ColorEdit4("Debug Colour", component.GetDebugColor().ToPtr());
		});

		DrawComponent<CardComponent>("Card", entity, [](auto& component)
		{
			ImGui::InputInt("Rank", &component.m_rank);

			ImGui::SelectableEnum("Suit", CardComponent::GetSuitNameList(), CardComponent::Suit::SUIT_COUNT, &((int)component.m_suit));

			if (component.GetCurrentSlot())
			{
				ImGui::Text("Card Slot");
				ImGui::Indent();
				ImGui::Text(component.GetCurrentSlot().GetName().c_str());
				ImGui::Unindent();
			}
		});

		DrawComponent<CardSlotComponent>("Card Slot", entity, [](auto& component)
		{
			ImGui::SelectableEnum("Slot Type", CardSlotComponent::GetSlotTypeNameList(), CardSlotComponent::SLOT_TYPE_COUNT, &((int)component.GetSlotTypeNonConst()));
			ImGui::SelectableEnum("Slot Layout", CardSlotComponent::GetSlotLayoutNameList(), CardSlotComponent::SLOT_LAYOUT_COUNT, &((int)component.GetSlotLayoutNonConst()));

			if (component.GetSlotLayout() == CardSlotComponent::SLOT_LAYOUT_STAGGERED)
			{
				ImGui::DragFloat2("Offset", component.GetStaggeredOffset().ToPtr(), 0.01f);
			}

			if (component.GetSlotType() == CardSlotComponent::SLOT_TYPE_SITE)
			{
				ImGui::Separator();
				ImGui::Indent();
				ImGui::SelectableEnum("Foundation Suit", CardComponent::GetSuitNameList(), CardComponent::Suit::SUIT_COUNT, &((int)component.m_siteInfo.m_foundationSuit));
				ImGui::InputInt("Foundation Rank", &component.m_siteInfo.m_foundationRank);
				ImGui::SelectableEnum("Rank Ordering", CardSlotComponent::sm_rankOrderingNameList, CardSlotComponent::RankOrdering::RANK_ORDERING_COUNT, &((int)component.m_siteInfo.m_rankOrdering));
				ImGui::SelectableEnum("Suit Ordering", CardSlotComponent::sm_suitOrderingNameList, CardSlotComponent::SuitOrdering::SUIT_ORDERING_COUNT, &((int)component.m_siteInfo.m_suitOrdering));
				ImGui::Checkbox("Can Loop", &component.m_siteInfo.m_canLoop);
				if (component.m_siteInfo.m_canLoop)
				{
					ImGui::InputInt("Rank", &component.m_siteInfo.m_loopMax);
				}
				ImGui::Unindent();
				ImGui::Separator();
			}

			if (component.GetSlotType() == CardSlotComponent::SLOT_TYPE_MONSTER)
			{
				Scene* entityScene = component.GetOwnerEntity().GetContext();
				Entity siteEntity = entityScene->GetEntityByUUID(component.m_monsterBattleSite);
				std::string entityName = ((EntityID)siteEntity != INVALID_ENTITY) ? siteEntity.GetName() : "None";
				char entityChar[128];
				std::strcpy(entityChar, entityName.c_str());
				ImGui::InputText("MonsterBattleSite", entityChar, sizeof(entityChar), ImGuiInputTextFlags_ReadOnly);

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_HIERACHY_UUID"))
					{
						UUID* uuid = (UUID*)payload->Data;
						component.m_monsterBattleSite = *uuid;
					}
					ImGui::EndDragDropTarget();
				}
			}

			{
				ImGui::Text("Card Stack");
				ImGui::BeginChild("Child", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 60), ImGuiChildFlags_Border);
				for (auto card : component.m_cardStack)
					ImGui::Text(card.GetName().c_str());
				ImGui::EndChild();
			}
		});

		DrawComponent<PatienceComponent>("Patience", entity, [](auto& component)
		{
			static char buffer[64];
			strcpy(buffer, component.m_setupScript.c_str());

			if (ImGui::InputText("Setup Script", buffer, sizeof(buffer)))
				component.m_setupScript = buffer;
		});
	}

	template<typename T>
	void SceneHierarchyPanel::DisplayAddComponentEntry(const std::string& entryName)
	{
		if (!m_selectionContext.HasComponent<T>())
		{
			if (ImGui::MenuItem(entryName.c_str()))
			{
				m_selectionContext.AddComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
	}
}