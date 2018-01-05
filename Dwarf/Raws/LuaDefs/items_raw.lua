-- Unprocessed items


items["ore"] = {
	name = "Ore",
	description = "Unprocessed ore",
	itemType = {"component"},
	glyph = glyphs["ore"],
	color = "grey",  
	tcode = 2,
	stockpile = stockpiles["ore"].id
};

items["metal_bar"] =  {
	name = "Metal Bar",
	description = "Forged in the fires of right over there..",
	itemType = {"component"},
	glyph = glyphs["metal_bars"],
	color = "grey",
	tcode = 2,
	stockpile = stockpiles["bars"].id
};

items["coal"] = {
	name = "Coal",
	description = "Freshly mined coal",
	itemType = {"component"},
	glyph = glyphs["stone_boulder"],
	color = "light black", 
	tcode = 2,
	stockpile = stockpiles["ore"].id
};

items["stone_boulder"] = {
	name = "A boulder",
	description = "A chunk of rock",
	itemType = {"component"},
	glyph = glyphs["stone_boulder"],
	color = "grey", 
	tcode = 2,
	stockpile = stockpiles["stone"].id
};

items["block"] = {
	name = "Block",
	description = "This is a block",
	itemType = {"component"},
	glyph = glyphs["block"],
	color = "brown",
	tcode = 2,
	stockpile = stockpiles["block"].id
};

items["anvil"] = {
	name = "Anvil",
	description = "Beat your metal against this",
	itemType = { "component" },
	glyph = glyphs["anvil"],
	color = "silver",
	stockpile = stockpiles["block"].id
};