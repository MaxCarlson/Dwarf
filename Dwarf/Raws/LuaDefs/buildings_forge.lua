
buildings["smelter"] = {
	name = "Smelter",
	description = "A white hot forge where metal is heated and poured",
	width = 3,
	height = 3,
	components = {{ item = "block", qty = 7 }},
	skill = { { name = "Construction", difficulty = 14 } }, 
	render = { { glyph = glyphs["w_tile"] }, { glyph = glyphs["m_smelter"] },  { glyph = glyphs["r_smelter"] }, 
			   { glyph = glyphs["bellow"] },    { glyph = glyphs["w_tile"] },     { glyph = glyphs["w_tile"] },
			   { glyph = glyphs["w_tile"] },    { glyph = glyphs["w_tile"] },   { glyph = glyphs["coal_pit"] } 
			 },
	structure
};


reactions["smelt_ore"] = { -- Should item = metal_bar be replaced with blocks? Just change render character in game based on mat type?
	name = "Smelt ore",
	workshop = "smelter",
	skill = "Furnace Operation",
	inputs = {{ item = "ore", qty = 1 }},
	outputs = {{ item = "block", qty = 1 }},
	difficulty = 10
};

buildings["wood_furnace"] = {
	name = "Wood Furnace",
	description = "Here wood can be burnt to produce charcoal",
	width = 3,
	height = 3,
	components = {{ item = "block", qty = 4 }},
	skill = {{ name = "Construction", difficulty = 12 }},
	render = { { glyph = glyphs["w_tile"] },       { glyph = glyphs["m_wood_burner"] }, { glyph = glyphs["r_wood_burner"] },
			   { glyph = glyphs["s_wood_stack"] }, { glyph = glyphs["w_tile"] },		{ glyph = glyphs["w_tile"] },
			   { glyph = glyphs["w_tile"] },       { glyph = glyphs["w_tile"] },		{ glyph = glyphs["coal_pit"] } },
	structure
};

reactions["make_charcoal"] = {
	name = "Make Charcoal",
	workshop = "wood_furnace",
	skill = "Furnace Operation",
	inputs = {{ item = "block", qty = 1, material = "wood" }}, -- Possibly use mat_type = organic ?
	outputs = {{ item = "charcoal", qty = 1 }},
	difficulty = 12
};

buildings["blacksmith"] = {
	name = "Blacksmith",
	description = "Here you can shape metal into things",
	width = 3,
	height = 3,
	components = {{ item = "block", qty = 7 }, { item = "anvil", qty = 14 }},
	skill = { { name = "Construction", difficulty = 1 } }, 
	render = { { glyph = glyphs["smith_top"] }, { glyph = glyphs["smith_mid"] },  { glyph = glyphs["w_tile"] }, 
			   { glyph = glyphs["smith_bottom"] },    { glyph = glyphs["w_tile"] },     { glyph = glyphs["w_tile"] },
			   { glyph = glyphs["coal_pit"] },    { glyph = glyphs["w_tile"] },   { glyph = glyphs["anvil"] } 
			 },
	structure
};

reactions["make_anvil"] = {
	name = "Make an Anvil",
	workshop = "smithy",
	skill = "blacksmithing",
	inputs = {{ item = "block", qty = 3, mat_type = metal }},
	outputs = {{ item = "anvil", qty = 1 }},
	difficulty = 16
};

reactions["make_pickaxe"] = {
	name = "Make Pickaxe",
	workshop = "smithy",
	skill = "blacksmithing",
	inputs = {{ item = "block", qty = 2, mat_type = metal }},
	outputs = {{ item = "pickaxe", qty = 1 }},
	difficulty = 17
};

