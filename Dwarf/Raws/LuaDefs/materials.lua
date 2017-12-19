materials = {


	--Switch Render system to a lookup based on lua traits ???

	-- Metal Ores prior to being mined
	["copper_ore"] = {
		name = "Copper Ore",
		description = "An reddish orange vein with streaks of green",
		color = "copper",
		glyph = 139,
		floorGlyph = 46,
		tcode = 2,
		health = 550,
		minesTo = "ore",
		mineAmt = 1,
		matType = { "rock" }
	};

	["tin_ore"] = {
		name = "Tin Ore",
		description = "A lightly silver-grey vein",
		color = "silver",
		glyph = 139,
		floorGlyph = 46,
		tcode = 2,
		health = 475,
		minesTo = "ore",
		mineAmt = 1,
		matType = { "rock" }
	};

	["iron_ore"] = {
		name = "Iron Ore",
		description = "Sparkly grey with hints of red",
		color = "iron",
		glyph = 139,
		floorGlyph = 46,
		tcode = 2,
		health = 625,
		minesTo = "ore",
		mineAmt = 1,
		matType = { "rock" }
	};

	-- Misc Ores prior to being mined

	["bitumous_coal"] = {
		name = "Bitumous coal",
		description = "A soft tar-like coal",
		color = "coal",
		glyph = 136,
		floorGlyph = 46,
		tcode = 2,
		health = 250,
		minesTo = "coal",
		mineAmt = 1,
		matType = { "rock" }
	};

	-- Garden variety stone prior to being mined

	["granite_rock"] = {
		name = "Granite",
		description = "A very hard and sparkly rock of many colors",
		color = "grey",
		floorGlyph = 46,
		glyph = 134, -- Add glyph index
		tcode = 2,
		health = 200,
		minesTo = "stone_boulder",
		mineAmt = 1,
		matType = { "rock" }
	};

	-- Placeholders in order to switch to new system of rendering

	["grass1"] = {
		color = "dark green",
		glyph = 130,
		floorGlyph = 130,
		tcode = 2,
		health = 200,
		matType = {"organic"}
	};

	["grass2"] = {
		color = "dark green",
		glyph = 147,
		floorGlyph = 147,
		tcode = 2,
		health = 200,
		matType = {"organic"}
	};

	["grass3"] = {
		color = "dark green",
		glyph = 244,
		floorGlyph = 244,
		tcode = 2,
		health = 200,
		matType = {"organic"}
	};
}

