#pragma once

#include "Rhombus.h"
#include "Patience/Components/PatienceComponent.h"
#include "Patience/Components/CardComponent.h"

#include <vector>

class PatienceSetupSystem : public System
{
public:
	struct CardData
	{
		const char* name;
		int rank = 0;
		CardComponent::Suit suit = CardComponent::Suit::SUIT_HEART;
		std::string sprite;

		CardData(const char* _cardName, int _rank, CardComponent::Suit _suit, std::string _sprite) :
			name(_cardName), rank(_rank), suit(_suit), sprite(_sprite) {}
	};

	PatienceSetupSystem(Scene* scene) : System(scene)
	{
	}

	void Init();

	void GetCardDataFromScript(const char* scriptName, std::vector<CardData>& cardDatas);
	void GetGameModeDataFromScript(const char* scriptName, PatienceComponent& patienceComponent);
};
