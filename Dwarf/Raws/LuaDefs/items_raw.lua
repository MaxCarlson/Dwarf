-- Unprocessed items


items["ore"] = {
	name = "Ore",
	description = "Unprocessed ore",
	itemType = {"component"},
	glyph = glyphs["ore"],
	foreground = "grey",
	background = "grey",
	stockpile = stockpiles["ore"].id
};

items["wood_log"] = {
	name = "Wood from a freshly downed tree",
	description = "Wood has a large number of uses",
	itemType = {"component"},
	glyph = glyphs["wood_logs"],
	foreground = "light brown",
	background = "grey",
	stockpile = stockpiles["wood"].id
};

items["coal"] = {
	name = "Coal",
	description = "Freshly mined coal",
	itemType = {"component"},
	glyph = glyphs["stone_boulder"],
	foreground = "light black",
	background = "grey",
	stockpile = stockpiles["ore"].id
};

items["charcoal"] = {
	name = "Charcoal",
	description = "A light weight black hydrocarbon",
	itemType = {"component"},
	glyph = glyphs["stone_boulder"],
	foreground = "light black",
	background = "grey",
	stockpile = stockpiles["fuel"].id
};

items["stone_boulder"] = {
	name = "A boulder",
	description = "A chunk of rock",
	itemType = {"component"},
	glyph = glyphs["stone_boulder"],
	foreground = "grey",
	background = "grey",
	stockpile = stockpiles["stone"].id
};

-- Used for representing a number of things,
-- from metal bars to proccessed stone. 
items["block"] = {
	name = "Block",
	description = "This is a block",
	itemType = {"component"},
	glyph = glyphs["block"],
	foreground = "grey",
	background = "grey",
	stockpile = stockpiles["block"].id
};

items["anvil"] = {
	name = "Anvil",
	description = "Beat your metal against this",
	itemType = { "component" },
	glyph = glyphs["anvil"],
	foreground = "silver",
	background = "grey",
	stockpile = stockpiles["component"].id
};