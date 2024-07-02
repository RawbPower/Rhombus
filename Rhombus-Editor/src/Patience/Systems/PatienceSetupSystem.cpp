#include "PatienceSetupSystem.h"

#include "Patience/Components/CardSlotComponent.h"
#include "Rhombus/Scripting/ScriptEngine.h"

#include <algorithm>
#include <random>

void PatienceSetupSystem::Init()
{
	std::vector<EntityID> cardSlots = m_scene->GetRegistry().GetEntityList<CardSlotComponent>();
	for (Entity entity : GetEntities())
	{
		// Get a list of columns slot to place the card into
		PatienceComponent& patienceComponent = entity.GetComponent<PatienceComponent>();
		std::vector<Entity> cardColumns;
		for (EntityID slot : cardSlots)
		{
			Entity cardSlotEntity = { slot, m_scene };
			CardSlotComponent& cardSlotComponent = cardSlotEntity.GetComponent<CardSlotComponent>();
			if (cardSlotComponent.GetSlotType() == CardSlotComponent::SlotType::SLOT_TYPE_COLUMN)
			{
				cardColumns.push_back(cardSlotEntity);
			}
		}

		std::string scriptPath = Project::GetScriptDirectory().string() + "\\GameModeData_" + patienceComponent.m_setupScript + ".lua";
		
		ScriptEngine::InitNewTable("GameModeData");
		if (ScriptEngine::DoScript(scriptPath))
		{
			GetGameModeDataFromScript(patienceComponent.m_setupScript.c_str(), patienceComponent);

			std::vector<CardData> cardDatas;
			GetCardDataFromScript(patienceComponent.m_setupScript.c_str(), cardDatas);

			auto rd = std::random_device{};
			auto rng = std::default_random_engine{ rd() };
			std::shuffle(std::begin(cardDatas), std::end(cardDatas), rng);

			int i = 0;
			for (CardData cardData : cardDatas)
			{
				Entity cardEntity = m_scene->CreateEntity(cardData.name);

				CardComponent& card = cardEntity.AddComponent<CardComponent>();
				card.m_rank = cardData.rank;
				card.m_suit = cardData.suit;

				BoxArea2DComponent& area = cardEntity.AddComponent<BoxArea2DComponent>();
				area.m_size = Vec2(22.5f, 32.0f);

				SpriteRendererComponent& spriteRendererComponent = cardEntity.AddComponent<SpriteRendererComponent>();
				std::string texturePath = cardData.sprite;
				auto path = Project::GetAssetFileSystemPath(texturePath);
				spriteRendererComponent.m_texture = Texture2D::Create(path.string());

				Entity cardColumnsEntity = { cardColumns[i], m_scene };
				CardSlotComponent& cardSlotComponent = cardColumnsEntity.GetComponent<CardSlotComponent>();
				cardSlotComponent.AddCard(cardEntity);
				card.SetCurrentlSlot(cardColumnsEntity);
				i = (i + 1) % cardColumns.size();
			}
		}

		for (EntityID slot : cardSlots)
		{
			Entity cardSlotEntity = { slot, m_scene };
			CardSlotComponent& cardSlotComponent = cardSlotEntity.GetComponent<CardSlotComponent>();
			cardSlotComponent.UpdateAllowCards();
		}
	}
}

void PatienceSetupSystem::GetGameModeDataFromScript(const char* scriptName, PatienceComponent& patienceComponent)
{
	std::list<std::string> cardSets;
	ScriptEngine::GetGlobal("GameModeData");
	ScriptEngine::GetField(scriptName);

	CardSlotComponent::InitCardSlotData();
}

void PatienceSetupSystem::GetCardDataFromScript(const char* scriptName, std::vector<CardData>& cardDatas)
{
	std::list<std::string> cardSets;
	ScriptEngine::GetGlobal("GameModeData");
	ScriptEngine::GetField(scriptName);
	ScriptEngine::GetListOfStringValueFromField("Deck", cardSets);

	ScriptEngine::InitNewTable("CardData");
	ScriptEngine::GetGlobal("CardData");
	for (std::string cardSet : cardSets)
	{
		std::string sciptPath = Project::GetScriptDirectory().string() + "\\CardData_" + cardSet + ".lua";
		if (ScriptEngine::DoScript(sciptPath.c_str()))
		{
			ScriptEngine::GetField(cardSet.c_str());
			ScriptEngine::StartLoopThroughTable();
			while (ScriptEngine::NextKey())
			{
				const char* name = ScriptEngine::GetKey();

				ScriptEngine::GetField("Sprite");
				std::string sprite = ScriptEngine::GetString();
				ScriptEngine::Pop();

				ScriptEngine::GetField("Rank");
				int rank = ScriptEngine::GetInt();
				ScriptEngine::Pop();

				ScriptEngine::GetField("Suit");
				CardComponent::Suit suit = (CardComponent::Suit)ScriptEngine::GetEnumFromName(ScriptEngine::GetString(), CardComponent::GetSuitNameList(), CardComponent::Suit::SUIT_COUNT);
				ScriptEngine::Pop();

				CardData cardData(name, rank, suit, sprite);
				cardDatas.push_back(cardData);
				ScriptEngine::Pop();
			}
			ScriptEngine::Pop();
		}
	}
}