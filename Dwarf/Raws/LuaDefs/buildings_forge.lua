
buildings["smelter"] = {
	name = "Smelter",
	description = "A white hot forge where metal is heated and poured",
	width = 3,
	height = 3,
	components = {{ item = "block", qty = 7 }},
	skill = { { name = "Construction", difficulty = 1 } }, 
	render = { { glyph = glyphs["w_tile"] }, { glyph = glyphs["m_smelter"] },  { glyph = glyphs["r_smelter"] }, 
			   { glyph = glyphs["bellow"] },    { glyph = glyphs["w_tile"] },     { glyph = glyphs["w_tile"] },
			   { glyph = glyphs["w_tile"] },    { glyph = glyphs["w_tile"] },   { glyph = glyphs["coal_pit"] } 
			 },
	structure
};


reactions["make_metal_bars"] = {
	name = "Make Metal Bars",
	workshop = "smelter",
	skill = "Smelting",
	inputs = {{ item = "ore", qty = 1 }},
	outputs = {{ item = "metal_bar", qty = 1 }}
};

buildings["smithy"] = {
	name = "Smithy",
	description = "Here you can shape metal bars into things",
	width = 3,
	height = 3,
	components = {{ item = "block", qty = 7 }, { item = "anvil", qty = 1 }},
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
	skill = "Smithing",
	inputs = {{ item = "metal_bar", qty = 3 }},
	outputs = {{ item = "anvil", qty = 1 }}
};

