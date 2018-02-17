-- Color of buildings will be based on material of construction

buildings = {
	
	bed = {
		name = "Bed",
		description = "You can sleep here!!",
		width = 1,
		height = 1,
		components = { { item = "bed", qty = 1 } },
		skill = { { name = "Construction", difficulty = 10 } }, 
		render = { { glyph = glyphs["bed"] } },
		structure,
		provides = {{ sleep }}
	},


}

