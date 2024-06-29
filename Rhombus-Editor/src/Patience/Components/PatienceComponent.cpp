#include "PatienceComponent.h"

const char* PatienceComponent::sm_revelationNameList[REVELATION_COUNT] = { "Open", "Closed" };
const char* PatienceComponent::sm_orderingNameList[ORDERING_COUNT] = { "Ascending", "Descending" };
const char* PatienceComponent::sm_packingTypeNameList[PACKING_COUNT] = { "Any", "DifferentSuit", "DifferentColor", "SameSuit"};
const char* PatienceComponent::sm_emptyColumnTypeNameList[EMPTY_COLUMN_COUNT] = { "Any", "King" };