
buildings["forge"] = {
	name = "Forge",
	description = "A white hot forge where metal is heated and pored",
	width = 2,
	height = 2,
	components = {{ item = "blocks", qty = 5 }},
	skill = { { name = "Construction", difficulty = 1 } }, 
	render = { { glyph = glyphs["barrel"] }, { glyph = glyphs["tools2"] }, { glyph = glyphs["nest_box"] }, { glyph = glyphs["placed_floor"] } },
	structure
};
