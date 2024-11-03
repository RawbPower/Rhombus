#include "GameEditorExtension.h"

#include "Rhombus.h"
#include "RhombusEditor.h"

#include "Components/CardComponent.h"
#include "Components/CardSlotComponent.h"
#include "Components/PatienceComponent.h"

void GameEditorExtension::DisplayAddComponentList(Entity entity)
{
	DisplayAddComponentEntry<CardComponent>("Card", entity);
	DisplayAddComponentEntry<CardSlotComponent>("Card Slot", entity);
	DisplayAddComponentEntry<PatienceComponent>("Patience", entity);
}

void GameEditorExtension::DisplayComponentProperties(Entity entity)
{
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