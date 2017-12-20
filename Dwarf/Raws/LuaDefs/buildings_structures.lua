-- Color of buildings will be based on material of construction

buildings = {
	
	wall = {
		name = "Wall",
		description = "I think you already know what a wall is..",
		width = 1,
		height = 1,
		components = { {item = "block", qty = 1} },
		provides = { wall = {}, floor = {} },
		skill = { name = "Construction", difficulty = 0 },
		render = { glyph = glyphs['wall'] },
		structure = 1
	},
};