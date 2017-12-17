materials = {}

--Switch Render system to a lookup based on lua traits

-- Misc Ores
materials["bitumous coal"] = {
	name = "Bitumous coal",
	description = "A soft tar-like coal",
	color = "coal",
	glyph = 136,
	tcode = 2,
	health = 250,
	minesTo = "coal",
	mineAmt = 1,
	matType = { "rock" }
};

-- Metal Ores
materials["copper_ore"] = {
	name = "Copper Ore",
	description = "An reddish orange vein with streaks of green",
	color = "copper",
	glyph = 139,
	tcode = 2,
	health = 550,
	minesTo = "ore",
	mineAmt = 1,
	matType = { "rock" }
};

materials["tin_ore"] = {
	name = "Tin Ore",
	description = "A lightly silver-grey vein",
	color = "silver",
	glyph = 139,
	tcode = 2,
	health = 475,
	minesTo = "ore",
	mineAmt = 1,
	matType = { "rock" }
};

materials["iron_ore"] = {
	name = "Iron Ore",
	description = "Sparkly grey with hints of red",
	color = "iron",
	glyph = 139,
	tcode = 2,
	health = 625,
	minesTo = "ore",
	mineAmt = 1,
	matType = { "rock" }
};

-- Garden variety stone

materials["granite"] = {
	name = "Granite",
	description = "A very hard and sparkly rock of many colors",
	color = "grey",
	glyph = 139, -- Add glyp index
	tcode = 2,
	health = 200,
	minesTo = "stoneBoulder",
	mineAmt = 1,
	matType = { "rock" }
};