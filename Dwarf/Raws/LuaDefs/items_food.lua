
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

items["potato"] = {
	name = "Potato",
	description = "A lumpy, brown root vegetable",
	itemType = {"food"},
	glyph = glyphs['leaf'],
	foreground = "brown",
	background = "brown",
	stockpile = stockpiles["food"].id
};

items["spinach"] = {
	name = "Spinach",
	description = "Very healthy!",
	itemType = {"food"},
	glyph = glyphs['leaf'],
	foreground = "dark grass_green",
	background = "green",
	stockpile = stockpiles["food"].id
};

items["strawberry"] = {
	name = "Strawberry",
	description = "A deliciously red berry",
	itemType = {"food"},
	glyph = glyphs['leaf'],
	foreground = "dark red",
	background = "red",
	stockpile = stockpiles["food"].id
};

items["turnip"] = {
	name = "Turnip",
	description = "A gross red vege",
	itemType = {"food"},
	glyph = glyphs['leaf'],
	foreground = "dark red",
	background = "red",
	stockpile = stockpiles["food"].id
};

items["soybean"] = {
	name = "Soybean",
	description = "A healthy bean thing",
	itemType = {"food"},
	glyph = glyphs['leaf'],
	foreground = "green",
	background = "dark green",
	stockpile = stockpiles["food"].id
};

items["plump_helmet"] = {
	name = "Plump Helmet",
	description = "Dwarves love these",
	itemType = {"food"},
	glyph = glyphs['leaf'],
	foreground = "purple",
	background = "dark green",
	stockpile = stockpiles["food"].id
};

-- Other items dropped by plants

items["devil_strand"] = {
	name = "Devil Strand",
	description = "A very red mushroom",
	itemType = {"component"},
	glyph = glyphs['leaf'],
	foreground = "red",
	background = "red",
	stockpile = stockpiles["component"].id
};

items["reeds"] = {
	name = "Reeds",
	description = "A thin, pliable tube",
	itemType = {""},
	glyph = glyphs['leaf'],
	foreground = "grass_green",
	background = "grey",
	stockpile = stockpiles["component"].id
};