
buildings["smelter"] = {
	name = "Smelter",
	description = "A white hot forge where metal is heated and pored",
	width = 3,
	height = 3,
	components = {{ item = "block", qty = 7 }},
	skill = { { name = "Construction", difficulty = 1 } }, 
	render = { { glyph = glyphs["w_tile"] }, { glyph = glyphs["m_smelter"] },  { glyph = glyphs["r_smelter"] }, 
			   { glyph = glyphs["bellow"] },    { glyph = glyphs["w_tile"] },     { glyph = glyphs["w_tile"] },
			   { glyph = glyphs["w_tile"] },    { glyph = glyphs["w_tile"] }, { glyph = glyphs["coal_pit"] } },
	structure
};


reactions["make_metal_bars"] = {
	name = "Make Metal Bars",
	workshop = "smelter",
	skill = "Smelting",
	inputs = {{ item = "ore", qty = 1 }},
	outputs = {{ item = "metal_bar", qty = 1 }}
};

