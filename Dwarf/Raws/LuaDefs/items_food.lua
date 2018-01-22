
-- Base seed items
items["seed"] = {
	name = "Seed",
	description = "Ready to be planted!",
	itemType = { "seed" },
	glyph = glyphs['seeds'],
	foreground = "grey",
	background =  "grey",
	stockpile = stockpiles["seeds"]
};


-- Food items dropped by plants

items["blackberry"] = {
	name = "Blackberry",
	description = "A juicy, black berry that grows on a prickly bush",
	itemType = { "food" },
	glyph = glyphs['leaf'],
	foreground = "purple",
	background = "white",
	stockpile = stockpiles["food"].id,
};