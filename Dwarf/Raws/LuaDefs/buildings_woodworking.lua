
buildings["sawmill"] = {
	name = "Sawmill",
	description = "Wood is cut to standardized sizes here",
	width = 3,
	height = 3,
	components = {{ item = "block", qty = 4 }},
	skill = { { name = "construction", difficulty = 14 } }, 
	render = { { glyph = glyphs["w_tile"] },    { glyph = glyphs["w_tile"] },  { glyph = glyphs["wood_planks"] }, 
			   { glyph = glyphs["saw_blade"] },    { glyph = glyphs["hor_ballista_bolt"] },     { glyph = glyphs["hor_ballista_bolt"] },
			   { glyph = glyphs["w_tile"] },    { glyph = glyphs["w_tile"] },   { glyph = glyphs["coiled_rope"] } 
			 },
	structure,
	provides = {{ workshop = true }}
};

-- Sawmill reactions

reactions["cut_wood_blocks"] = {
	name = "Cut wood into planks",
	workshop = "sawmill",
	skill = "carpentry",
	inputs = {{ item = "wood_log", qty = 1 }},
	outputs = {{ item = "block", qty = 3 }},
	difficulty = 10,
	time = 3
};

