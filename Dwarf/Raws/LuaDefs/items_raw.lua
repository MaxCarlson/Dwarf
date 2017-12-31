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
