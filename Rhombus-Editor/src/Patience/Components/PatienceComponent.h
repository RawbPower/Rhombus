#pragma once

#include "Rhombus.h"

class PatienceComponent : public ComponentBase
{
public:
	enum Revelation { REVELATION_OPEN = 0, REVELATION_CLOSED, REVELATION_COUNT };
	enum Ordering { ORDERING_ASCENDING = 0, ORDERING_DESCENDING, ORDERING_COUNT };
	enum PackingType { PACKING_ANY, PACKING_DIFFERENT_SUIT, PACKING_DIFFERENT_COLOR, PACKING_SAME_SUIT, PACKING_COUNT };
	enum EmptyColumnType { EMPTY_COLOUMN_ANY, EMPTY_COLUMN_KING, EMPTY_COLUMN_COUNT };

	struct GameModeData
	{
		int columns;
		int sites;
		int freecells;
		int stocks;
		int wastepiles;

		Revelation revelation;

		Ordering buildingOrder;
		int foundationRank;
		Ordering packingOrder;
		PackingType packingType;
		EmptyColumnType emptyColumnType;
	};

	PatienceComponent() = default;
	PatienceComponent(const PatienceComponent&) = default;

	std::string m_setupScript;
	GameModeData m_gameModeData;

	static const char* sm_revelationNameList[REVELATION_COUNT];
	static const char* sm_orderingNameList[ORDERING_COUNT];
	static const char* sm_packingTypeNameList[PACKING_COUNT];
	static const char* sm_emptyColumnTypeNameList[EMPTY_COLUMN_COUNT];
};
