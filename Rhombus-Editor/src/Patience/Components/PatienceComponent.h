#pragma once

#include "Rhombus.h"

class PatienceComponent : public ComponentBase
{
public:
	struct GameModeData
	{
		enum RevelationType {REVELATION_OPEN = 0, REVELATION_CLOSED};
		enum OrderingType {ORDERING_ASCENDING = 0, ORDERING_DESCENDING};
		enum PackingType {PACKING_ANY, PACKING_DIFFERENT_SUIT, PACKING_DIFFERENT_COLOR, PACKING_SAME_SUIT};
		enum EmptyColumnType {EMPTY_COLOUMN_ANY, EMPTY_COLUMN_KING};

		int columns;
		int sites;
		int freecells;
		int stocks;
		int wastepiles;

		RevelationType revelation;

		OrderingType buildingOrder;
		int foundationRank;
		OrderingType packingOrder;
		PackingType packingType;
		EmptyColumnType emptyColumnType;
	};

	PatienceComponent() = default;
	PatienceComponent(const PatienceComponent&) = default;

	std::string m_setupScript;

private:
	GameModeData m_gameModeData;
};
