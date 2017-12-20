materials = {


	--Switch Render system to a lookup based on lua traits ???

	-- Metal Ores prior to being mined
	["copper_ore"] = {
		name = "Copper Ore",
		layer = "igneous",
		description = "An reddish orange vein with streaks of green",
		color = "copper",
		glyph = glyphs['med_stone'], 
		floorGlyph = 46,
		tcode = 2,
		health = 550,
		minesTo = "ore",
		ores = "copper",
		mineAmt = 1,
		matType = { "rock" }
	};

	["tin_ore"] = {
		name = "Tin Ore",
		layer = "sedimentary",
		description = "A lightly silver-grey vein",
		color = "silver",
		glyph = glyphs['med_stone'], 
		floorGlyph = 46,
		tcode = 2,
		health = 475,
		minesTo = "ore",
		ores = "tin",
		mineAmt = 1,
		matType = { "rock" }
	};

	-- Iron producers
	["iron_ore"] = {
		name = "Iron Ore",
		layer = "igneous",
		description = "Sparkly grey with hints of red",
		color = "iron",
		glyph = glyphs['med_stone'], 
		floorGlyph = 46,
		tcode = 2,
		health = 625,
		minesTo = "ore",
		ores = "iron",
		mineAmt = 1,
		matType = { "rock" }
	};

	["hematite_s"] = {
		name = "Hematite",
		layer = "sedimentary",
		description = "An iron rich ore",
		color = "light red",
		glyph = glyphs['med_stone'], 
		floorGlyph = 46,
		tcode = 2,
		health = 625,
		minesTo = "ore",
		ores = "iron",
		mineAmt = 1,
		matType = { "rock" }
	};

	-- Titanium containers -- Add Ilmenite?
	["rutile"] = {
		name = "Rutile",
		layer = "metamorphic",
		description = "A dark-reddish ore with a high Titanium concentration",
		color = "purple",
		glyph = glyphs['val_stone'],
		floorGlyph = 46,
		tcode = 2,
		health = 625,
		minesTo = "ore",
		ores = "titanium",
		mineAmt = 1,
		matType = { "rock" }
	};

	

	["native_gold"] = {
		name = "Native Gold",
		layer = "igneous",
		description = "Worth it's weight in gold",
		color = "dark yellow",
		glyph = glyphs['val_stone'],
		floorGlyph = 46,
		tcode = 2,
		health = 180,
		minesTo = "ore",
		ores = "gold",
		mineAmt = 1,
		matType = { "rock" }
	};

	-- Misc Ores prior to being mined

	["bitumous_coal"] = {
		name = "Bitumous coal",
		layer = "sedimentary",
		description = "A soft tar-like coal",
		color = glyphs['coal_stone'],
		glyph = 136,
		floorGlyph = 46,
		tcode = 2,
		health = 250,
		minesTo = "coal",
		mineAmt = 1,
		matType = { "rock" }
	};
	
	-- Flux stones -- Have to add in catagory
	["marble"] = {
		name = "Marble",
		layer = "sedimentary",
		description = "Making a sculputre are we McDwarf?",
		color = "white",
		glyph = glyphs['flux_stone'],
		floorGlyph = 46,
		tcode = 2,
		health = 145,
		minesTo = "stone_boulder",
		mineAmt = 1,
		matType = { "rock" }
	};

	["limestone"] = {
		name = "Limestone",
		layer = "sedimentary",
		description = "Is that a skeleton?",
		color = "lightest amber",
		glyph = glyphs['flux_stone'],
		floorGlyph = 46,
		tcode = 2,
		health = 120,
		minesTo = "stone_boulder",
		mineAmt = 1,
		matType = { "rock" }
	};

	-- Garden variety stone prior to being mined

	["granite_rock"] = {
		name = "Granite",
		layer = "igneous",
		description = "A very hard and sparkly rock of many colors",
		color = "grey",
		glyph = glyphs['stone4'],
		floorGlyph = 46,
		tcode = 2,
		health = 200,
		minesTo = "stone_boulder",
		mineAmt = 1,
		matType = { "rock" }
	};

	["microcline"] = {
		name = "microcline",
		layer = "igneous",
		description = "A hard blue rock",
		color = "light blue",
		glyph = glyphs['stone2'], 
		floorGlyph = 46,
		tcode = 2,
		health = 190,
		minesTo = "stone_boulder",
		mineAmt = 1,
		matType = { "rock" }
	};

	["olivine"] = {
		name = "Olivine",
		layer = "igneous",
		description = "A pleasent green crystally rock",
		color = "green",
		glyph = glyphs['stone2'], 
		floorGlyph = 46,
		tcode = 2,
		health = 120,
		minesTo = "stone_boulder",
		mineAmt = 1,
		matType = { "rock" }
	};

	["mica"] = {
		name = "Mica",
		layer = "all",
		description = "A flakey stone found just about everywhere",
		color = "lightest amber",
		glyph = glyphs['stone2'],
		floorGlyph = 46, 
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
		glyph = glyphs['soil1'], -- Add glyph index
		floorGlyph = 126,
		tcode = 2,
		health = 200,
		matType = { "soil" }	
	};

	-- Placeholders in order to switch to new system of rendering

	["grass1"] = {
		color = "dark green",
		glyph = glyphs['grass1'], 
		floorGlyph = 130,
		tcode = 2,
		health = 200,
		matType = {"organic"}
	};

	["grass2"] = {
		color = "dark green",
		glyph = glyphs['grass2'], 
		floorGlyph = 147,
		tcode = 2,
		health = 200,
		matType = {"organic"}
	};

	["grass3"] = {
		color = "dark green",
		glyph = glyphs['grass3'], 
		floorGlyph = 244,
		tcode = 2,
		health = 200,
		matType = {"organic"}
	};
}

