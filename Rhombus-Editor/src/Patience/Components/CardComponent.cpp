#include "CardComponent.h"

const char* CardComponent::sm_suitNameList[SUIT_COUNT] = { "Heart", "Diamond", "Spade", "Club" };

CardComponent::Suit CardComponent::GetSuitFromName(const char* name)
{
	CardComponent::Suit suit = CardComponent::SUIT_COUNT;
	int suitIndex = 0;
	for (const char* suitName : sm_suitNameList)
	{
		if (suitName == name)
		{
			suit = (CardComponent::Suit)suitIndex;
			break;
		}
		suitIndex++;
	}

	return suit;
}