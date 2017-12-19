materials = {


	--Switch Render system to a lookup based on lua traits ???

	-- Metal Ores prior to being mined
	["copper_ore"] = {
		name = "Copper Ore",
		layer = "igneous",
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
		layer = "sedimentary",
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
		layer = "igneous",
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

	["gold_ore"] = {
		name = "Gold ore",
		layer = "igneous",
		description = "Worth it's weight in gold",
		color = "yellow",
		glyph = 138,
		floorGlyph = 46,
		tcode = 2,
		health = 180,
		minesTo = "ore",
		mineAmt = 1,
		matType = { "rock" }
	};

	-- Misc Ores prior to being mined

	["bitumous_coal"] = {
		name = "Bitumous coal",
		layer = "sedimentary",
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
		layer = "igneous"
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

	["marble"] = {
		name = "Marble",
		layer = "sedimentary",
		description = "Making a sculputre are we McDwarf?"
		color = "white",
		floorGlyph = 46,
		glyph = 137,
		tcode = 2,
		health = 145,
		minesTo = "stone_boulder",
		mineAmt = 1,
		matType = { "rock" }
	};

	["microcline"] = {
		name = "microcline",
		layer = "igneous",
		description = "A hard blue rock",
		color = "light blue",
		floorGlyph = 46,
		glyph = 132, 
		tcode = 2,
		health = 190,
		minesTo = "stone_boulder",
		mineAmt = 1,
		matType = { "rock" }
	};

	["olivine"] = {
		name = "Olivine"
		layer = "igneous",
		description = "A pleasent green crystally rock",
		color = "green",
		floorGlyph = 46,
		glyph = 132, 
		tcode = 2,
		health = 120,
		minesTo = "stone_boulder",
		mineAmt = 1,
		matType = { "rock" }
	};

	-- Soils

	["fertile_soil"] = {
		name = "Fertile Soil",
		layer = "soil",
		description = "Dark, rich, fertile soil",
		color = "brown",
		floorGlyph = 126,
		glyph = 142, -- Add glyph index
		tcode = 2,
		health = 200,
		matType = { "soil" }	
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

