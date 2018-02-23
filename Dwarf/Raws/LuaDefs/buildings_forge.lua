
buildings["smelter"] = {
	name = "Smelter",
	description = "A white hot forge where metal is heated and poured",
	width = 3,
	height = 3,
	components = {{ item = "block", qty = 7 }},
	skill = { { name = "construction", difficulty = 14 } }, 
	render = { { glyph = glyphs["w_tile"] }, { glyph = glyphs["m_smelter"] },  { glyph = glyphs["r_smelter"] }, 
			   { glyph = glyphs["bellow"] },    { glyph = glyphs["w_tile"] },     { glyph = glyphs["w_tile"] },
			   { glyph = glyphs["w_tile"] },    { glyph = glyphs["w_tile"] },   { glyph = glyphs["coal_pit"] } 
			 },
	structure,
	provides = {{ workshop = true }}
};


reactions["smelt_ore"] = { -- Should item = metal_bar be replaced with blocks? Just change render character in game based on mat type?
	name = "Smelt ore",
	workshop = "smelter",
	skill = "furnace_operation",
	inputs = {{ item = "ore", qty = 1 }},
	outputs = {{ item = "block", qty = 1 }},
	difficulty = 10,
	time = 3
};

buildings["wood_furnace"] = {
	name = "Wood Furnace",
	description = "Here wood can be burnt to produce charcoal",
	width = 3,
	height = 3,
	components = {{ item = "block", qty = 4 }},
	skill = {{ name = "construction", difficulty = 12 }},
	render = { { glyph = glyphs["w_tile"] },       { glyph = glyphs["m_wood_burner"] }, { glyph = glyphs["r_wood_burner"] },
			   { glyph = glyphs["s_wood_stack"] }, { glyph = glyphs["w_tile"] },		{ glyph = glyphs["w_tile"] },
			   { glyph = glyphs["w_tile"] },       { glyph = glyphs["w_tile"] },		{ glyph = glyphs["coal_pit"] } },
	structure,
	provides = {{ workshop = true }}
};

reactions["make_charcoal"] = {
	name = "Make Charcoal",
	workshop = "wood_furnace",
	skill = "furnace_operation",
	inputs = {{ item = "block", qty = 1, material = "wood" }}, -- Possibly use mat_type = organic ?
	outputs = {{ item = "charcoal", qty = 2 }},
	difficulty = 12,
	time = 3
};

buildings["blacksmith"] = {
	name = "Blacksmith",
	description = "Here you can shape metal into things",
	width = 3,
	height = 3,
	components = {{ item = "block", qty = 7 }, { item = "anvil", qty = 1 }},
	skill = { { name = "construction", difficulty = 1 } }, 
	render = { { glyph = glyphs["smith_top"] },    { glyph = glyphs["smith_mid"] },  { glyph = glyphs["w_tile"] }, 
			   { glyph = glyphs["smith_bottom"] }, { glyph = glyphs["w_tile"] },     { glyph = glyphs["w_tile"] },
			   { glyph = glyphs["coal_pit"] },     { glyph = glyphs["w_tile"] },     { glyph = glyphs["anvil"] } 
			 },
	structure,
	provides = {{ workshop = true }}
};

reactions["make_anvil"] = {
	name = "Make an Anvil",
	workshop = "blacksmith",
	skill = "blacksmithing",
	inputs = {{ item = "block", qty = 3, mat_type = "metal" }},
	outputs = {{ item = "anvil", qty = 1 }},
	difficulty = 16,
	time = 10
};

reactions["make_pickaxe"] = {
	name = "Make Pickaxe",
	workshop = "blacksmith",
	skill = "blacksmithing",
	inputs = {{ item = "block", qty = 2, mat_type = "metal" }},
	outputs = {{ item = "pickaxe", qty = 1 }},
	difficulty = 17,
	time = 12
};

reactions["make_hatchet"] = {
	name = "Make Hatchet",
	workshop = "blacksmith",
	skill = "blacksmithing",
	inputs = {{ item = "block", qty = 1, mat_type = "metal" }},
	outputs = {{ item = "hatchet", qty = 1 }},
	difficulty = 15,
	time = 10
};

-- Weapons

reactions["make_short_sword"] = {
	name = "Make Short Sword",
	workshop = "blacksmith",
	skill = "blacksmithing",
	inputs = {{ item = "block", qty = 2, mat_type = "metal" }},
	outputs = {{ item = "short_sword", qty = 1 }},
	difficulty = 14,
	time = 10
};

reactions["make_bastard_sword"] = {
	name = "Make Bastard Sword",
	workshop = "blacksmith",
	skill = "blacksmithing",
	inputs = {{ item = "block", qty = 3, mat_type = "metal" }},
	outputs = {{ item = "bastard_sword", qty = 1 }},
	difficulty = 15,
	time = 12
};

reactions["make_axe"] = {
	name = "Make Axe",
	workshop = "blacksmith",
	skill = "blacksmithing",
	inputs = {{ item = "block", qty = 4, mat_type = "metal" }},
	outputs = {{ item = "axe", qty = 1 }},
	difficulty = 15,
	time = 13
};

-- 2 Handed weapons
reactions["make_long_sword"] = {
	name = "Make Long Sword",
	workshop = "blacksmith",
	skill = "blacksmithing",
	inputs = {{ item = "block", qty = 6, mat_type = "metal" }},
	outputs = {{ item = "long_sword", qty = 1 }},
	difficulty = 18,
	time = 19
};

reactions["make_battle_axe"] = {
	name = "Make Battle Axe",
	workshop = "blacksmith",
	skill = "blacksmithing",
	inputs = {{ item = "block", qty = 7, mat_type = "metal" }},
	outputs = {{ item = "axe", qty = 1 }},
	difficulty = 18,
	time = 20
};