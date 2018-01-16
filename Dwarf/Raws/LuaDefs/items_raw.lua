-- Unprocessed items


items["ore"] = {
	name = "Ore",
	description = "Unprocessed ore",
	itemType = {"component"},
	glyph = glyphs["ore"],
	color = "grey",  
	stockpile = stockpiles["ore"].id
};

items["wood_log"] = {
	name = "Wood from a freshly downed tree",
	description = "Wood has a large number of uses",
	itemType = {"component"},
	glyph = glyphs["wood_logs"],
	color = "light brown",
	stockpile = stockpiles["wood"].id
};

items["coal"] = {
	name = "Coal",
	description = "Freshly mined coal",
	itemType = {"component"},
	glyph = glyphs["stone_boulder"],
	color = "light black", 
	stockpile = stockpiles["ore"].id
};

items["charcoal"] = {
	name = "Charcoal",
	description = "A light weight black hydrocarbon",
	itemType = {"component"},
	glyph = glyphs["stone_boulder"],
	color = "light black",
	stockpile = stockpiles["fuel"].id
};

items["stone_boulder"] = {
	name = "A boulder",
	description = "A chunk of rock",
	itemType = {"component"},
	glyph = glyphs["stone_boulder"],
	color = "grey", 
	stockpile = stockpiles["stone"].id
};

-- Used for representing a number of things,
-- from metal bars to proccessed stone. 
items["block"] = {
	name = "Block",
	description = "This is a block",
	itemType = {"component"},
	glyph = glyphs["block"],
	color = "brown",
	stockpile = stockpiles["block"].id
};

items["anvil"] = {
	name = "Anvil",
	description = "Beat your metal against this",
	itemType = { "component" },
	glyph = glyphs["anvil"],
	color = "silver",
	stockpile = stockpiles["component"].id
};