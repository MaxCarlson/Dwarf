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
		provides = {{ sleep = 1 }, { comfort = 1 }} -- This should probably add comfort?
	},

	table = {
		name = "Table",
		description = "Nice and flat.",
		width = 1,
		height = 1,
		components = { { item = "table", qty = 1 } },
		skill = { { name = "Construction", difficulty = 10 } }, 
		render = { { glyph = glyphs["table"] } },
		structure,
		provides = {{ table = 1 }}
	},
	
	chair = {
		name = "Chair",
		description = "Sit here!",
		width = 1,
		height = 1,
		components = { { item = "chair", qty = 1 } },
		skill = { { name = "Construction", difficulty = 10 } }, 
		render = { { glyph = glyphs["chair"] } },
		structure,
		provides = {{ chair = 1 }, { comfort = 1 } }
	},
}

