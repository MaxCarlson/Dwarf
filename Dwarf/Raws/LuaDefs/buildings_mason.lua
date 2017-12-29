


buildings["mason"] = {
	name = "Mason",
	description = "A building where stone is worked",
	width = 2,
	height = 2,
	components = { { item = "stone_boulder", qty = 3 } },
	skill = { { name = "Construction", difficulty = 0 } }, 
	render = { { glyph = 123 }, { glyph = 111 }, { glyph = 8 }, { glyph = 43 } },
	structure
};


-- Reactions for the mason building
reactions = {

	make_stone_blocks = {
		name = "Make Stone Blocks",
		workshop = "mason",
		skill = "Masonry",
		inputs = {{ item = "stone_boulder", qty = 1 }},
		outputs = {{ item = "block", qty = 3 }}
	},

	make_stone_door = {
		name = "Make Stone Door",
		workshop = "mason",
		skill = "Masonry",
		inputs = {{ item = "stone_boulder", qty = 1 }},
		outputs = {{ item = "door", qty = 1}}
	},

	make_statue = {
		name = "Make Statue",
		workshop = "mason",
		skill = "Masonry",
		inputs = {{ item = "stone_boulder", qty = 1 }},
		outputs = {{ item = "statue", qty = 1}}
	},
};