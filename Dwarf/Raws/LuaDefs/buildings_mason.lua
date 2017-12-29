
buildings = {

	mason = {
		name = "Mason",
		description = "A building where stone is worked",
		width = 2,
		height = 2,
		components = { { item = "stone_boulder", qty = 3 } },
		skill = { { name = "Construction", difficulty = 0 } }, 
		render = { { glyph = 123, glyph1 = 111, glyph2 = 8, glyph3 = 43 } },
		structure
	}
};

-- Reactions for the mason building
reactions = {

	make_stone_blocks = {
		name = "Make Stone Blocks",
		workshop = "mason",
		skill = "Masonry",
		inputs = {{ item = "stone_boulder", qty = 1 }, { item="clay", qty=3} },
		--outputs = {{item = "block", qty = 3 }}
	}


}