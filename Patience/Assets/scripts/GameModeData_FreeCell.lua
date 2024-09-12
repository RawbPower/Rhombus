GameModeData.FreeCell =
{
	Columns = 8,
	Sites = 4,
	Freecells = 4,
	Stocks = 0,
	Wastepiles = 0,

	Revelation = "Open",

	--[[BuildingRankOrder = "Ascending",
	BuildingType = "Any"
	FoundationRank = "Any", ]]--
	PackingRankOrder = "Descending",
	PackingSuitOrder = "DifferentSuit",
	EmptyColumnType = "Any",

	MainDiscard = "Discard",
	MonsterDiscard = "Graveyard",

	Deck = 
	{
		"BasicHearts",
		"BasicDiamonds",
		"BasicSpades",
		"BasicClubs",
		"FaceMonsters"
	},

	StartingColumns =
	{
		"Column 1",
		"Column 2",
		"Column 3",
		"Column 4",
		"Column 5",
		"Column 6",
		"Column 7",
		"Column 8"
	}
}