
materials = {
	
	-- Pure Metals
	["copper"] = {
		name = "Copper",
		description = "A semi-soft metal that turns green over time",
		color = "copper",
		glyph = glyphs["metal_bars"],
		floorGlyph = glyphs["placed_floor"],
		health = 790,
	};

	["tin"] = {
		name = "Tin",
		description = "Very weak silvery metal",
		color = "silver",
		glyph = glyphs["metal_bars"],
		floorGlyph = glyphs["placed_floor"],
		health = 500,
	};

	["iron"] = {
		name = "Iron",
		description = "A hard to brittle dark colored metal",
		color = "iron",
		glyph = glyphs["metal_bars"],
		floorGlyph = glyphs["placed_floor"],
		health = 950,		
	};

	["titanium"] = {
		name = "Titanium",
		description = "A very hard very light metal",
		color = "silver",
		glyph = glyphs["metal_bars"],
		floorGlyph = glyphs["placed_floor"],
		health = 1600,
	};

	["gold"] = {
		name = "Gold",
		description = "A very soft much sought after metal",
		color = "gold",
		glyph = glyphs["metal_bars"],
		floorGlyph = glyphs["placed_floor"],
		health = 450,
	};

	-- Metal Ores prior to being mined
	["native_copper"] = {
		name = "Native copper",
		layer = "igneous",
		description = "An reddish orange vein with streaks of green",
		color = "copper",
		glyph = glyphs['med_stone'], 
		floorGlyph = glyphs['floor2'],
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
		floorGlyph = glyphs['floor2'],
		health = 475,
		minesTo = "ore",
		ores = "tin",
		mineAmt = 1,
		matType = { "rock" }
	};

	-- Iron producers
	["magnetite"] = {
		name = "Magnetite",
		layer = "igneous",
		description = "Sparkly grey with hints of orange",
		color = "light iron",
		glyph = glyphs['med_stone'], 
		floorGlyph = glyphs['floor2'],
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
		floorGlyph = glyphs['floor2'],
		health = 625,
		minesTo = "ore",
		ores = "iron",
		mineAmt = 1,
		matType = { "rock" }
	};

	["limonite"] = {
		name = "Limonite",
		layer = "sedimentary",
		description = "A yellowish iron containing ore",
		color = "amber",
		glyph = glyphs['med_stone'], 
		floorGlyph = glyphs['floor2'],
		health = 425,
		minesTo = "ore",
		ores = "iron",
		mineAmt = 1,
		matType = { "rock" } 
	};

	["goethite"] = {
		name = "Goethite",
		layer = "sedimentary",
		description = "An ore heavy in iron, traditionally used in ochre pigments",
		color = "silver",
		glyph = glyphs['med_stone'], 
		floorGlyph = glyphs['floor2'],
		health = 425,
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
		color = "light purple_blue",
		glyph = glyphs['val_stone'],
		floorGlyph = glyphs['floor2'],
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
		color = "gold",
		glyph = glyphs['val_stone'],
		floorGlyph = glyphs['floor2'],
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
		color = "dark grey",
		glyph = glyphs['coal_stone'],
		floorGlyph = glyphs['floor1'],
		health = 250,
		minesTo = "coal",
		mineAmt = 1,
		matType = { "rock" }
	};
	
	-- Flux stones -- Have to add in catagory
	["marble"] = {
		name = "Marble",
		layer = "metamorphic",
		description = "Making a sculputre are we McDwarf?",
		color = "white",
		glyph = glyphs['flux_stone'],
		floorGlyph = glyphs['floor1'],
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
		floorGlyph = glyphs['floor1'],
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
		floorGlyph = glyphs['floor1'],
		health = 200,
		minesTo = "stone_boulder",
		mineAmt = 1,
		matType = { "rock" }
	};

	["gneiss"] = {
		name = "Gneiss",
		layer = "metamorphic",
		description = "A rough, banded rock",
		color = "lightest red",
		glyph = glyphs['stone4'],
		floorGlyph = glyphs['floor1'],
		health = 200,
		minesTo = "stone_boulder",
		mineAmt = 1,
		matType = { "rock" }
	};

	["quartzite"] = {
		name = "Quartzite",
		layer = "metamorphic",
		description = "Ranging in color from red to blue, crystally rock",
		color = "light blue",
		glyph = glyphs['stone2'], 
		floorGlyph = glyphs['floor1'],
		health = 190,
		minesTo = "stone_boulder",
		mineAmt = 1,
		matType = { "rock" }
	};

	["serpentinite"] = {
		name = "Serpentinite",
		layer = "metamorphic",
		description = "A dark green banded rock",
		color = "dark green",
		glyph = glyphs['stone2'], 
		floorGlyph = glyphs['floor1'],
		health = 190,
		minesTo = "stone_boulder",
		mineAmt = 1,
		matType = { "rock" }
	};

	["slate"] = {
		name = "Slate",
		layer = "metamorphic",
		description = "Comes in a wide variety of colors",
		color = "darkest grey",
		glyph = glyphs['stone4'],
		floorGlyph = glyphs['floor1'],
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
		floorGlyph = glyphs['floor1'],
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
		floorGlyph = glyphs['floor1'],
		health = 120,
		minesTo = "stone_boulder",
		mineAmt = 1,
		matType = { "rock" }
	};

	["mica"] = {
		name = "Mica",
		layer = "all_rock",
		description = "A flakey stone found just about everywhere",
		color = "lightest amber",
		glyph = glyphs['stone2'],
		floorGlyph = glyphs['floor1'], 
		health = 120,
		minesTo = "stone_boulder",
		mineAmt = 1,
		matType = { "rock" }		

	};

	-- Soils and Sands

	["fertile_soil"] = {
		name = "Fertile Soil",
		layer = "soil",
		description = "Dark, rich, fertile soil",
		color = "brown",
		glyph = glyphs['soil1'], -- Add glyph index
		floorGlyph = glyphs['floor1'],
		health = 200,
		matType = { "soil" }	
	};

	["yellow_sand"] = {
		name = "Yellow Sand",
		layer = "soil",
		description = "A fine, yellow sand.",
		color = "yellow",
		glyph = glyphs['sand1'],
		floorGlyph = glyphs['floor1'],
		health = 140,
		matType = { "sand" }
	};

	-- Misc materials

	["wood"] = {
		name = "Wood",
		description = "Wood cut from a tree",
		color = "brown",
		floorGlyph = glyphs["placed_floor"],
		health = 250,
		matType = { "synthetic" }
	};

	["organic"] = {
		name = "Organic",
		color = "green",
		health = 10,
		matType = {"organic"},
	};

	["food"] = {
		name = "Food",
		color = "green",
		health = 10,
		matType = {"food"}
	};

	["seed"] = {
		name = "Seed",
		color = "grey",
		health = 5,
		matType = {"organic"}
	};
}

