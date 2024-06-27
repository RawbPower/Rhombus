#include "PatienceSetupSystem.h"

#include "Patience/Components/PatienceComponent.h"
#include "Patience/Components/CardSlotComponent.h"
#include "Patience/Components/CardComponent.h"
#include "Rhombus/Scripting/ScriptEngine.h"


void PatienceSetupSystem::Init()
{

	for (Entity entity : GetEntities())
	{
		PatienceComponent& patienceComponent = entity.GetComponent<PatienceComponent>();
		std::vector<EntityID> cardSlots = m_scene->GetRegistry().GetEntityList<CardSlotComponent>();
		std::vector<Entity> cardColumns;
		for (EntityID slot : cardSlots)
		{
			Entity cardSlotEntity = { slot, m_scene };
			CardSlotComponent& cardSlotComponent = cardSlotEntity.GetComponent<CardSlotComponent>();
			if (cardSlotComponent.GetSlotType() == CardSlotComponent::STAGGERED)
			{
				cardColumns.push_back(cardSlotEntity);
			}
		}

		std::string scriptPath = Project::GetScriptDirectory().string() + "\\GameModeData_" + patienceComponent.m_setupScript + ".lua";
		
		if (ScriptEngine::DoScript(scriptPath))
		{
			std::list<std::string> cardData = ScriptEngine::GetCardsTest(patienceComponent.m_setupScript);

			int i = 0;
			for (std::string cardName : cardData)
			{
				Entity cardEntity = m_scene->CreateEntity(cardName);
				CardComponent& card = cardEntity.AddComponent<CardComponent>();
				BoxArea2DComponent& area = cardEntity.AddComponent<BoxArea2DComponent>();
				area.m_size = Vec2(22.5f, 32.0f);
				SpriteRendererComponent& spriteRendererComponent = cardEntity.AddComponent<SpriteRendererComponent>();
				std::string texturePath = "textures\\Cards\\Card1.png";
				auto path = Project::GetAssetFileSystemPath(texturePath);
				spriteRendererComponent.m_texture = Texture2D::Create(path.string());

				Entity cardColumnsEntity = { cardColumns[i], m_scene };
				CardSlotComponent& cardSlotComponent = cardColumnsEntity.GetComponent<CardSlotComponent>();
				cardSlotComponent.AddCard(cardEntity);
				card.SetCurrentlSlot(cardColumnsEntity);
				i = (i + 1) % cardColumns.size();
			}
		}
	}
}