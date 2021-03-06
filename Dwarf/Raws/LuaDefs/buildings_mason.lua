

buildings["mason"] = {
	name = "Mason",
	description = "A building where stone is worked",
	width = 2,
	height = 2,
	components = { { item = "stone_boulder", qty = 3 } },
	skill = { { name = "Construction", difficulty = 10 } }, 
	render = { { glyph = glyphs["barrel"] }, { glyph = glyphs["tools2"] }, { glyph = glyphs["nest_box"] }, { glyph = glyphs["placed_floor"] } },
	structure,
	provides = {{ workshop = true }}
};


-- Reactions for the mason building
reactions = {

	make_stone_blocks = {
		name = "Stone Blocks",
		workshop = "mason",
		skill = "masonry",
		inputs = {{ item = "stone_boulder", qty = 1 }},
		outputs = {{ item = "block", qty = 3 }},
		difficulty = 7,
		time = 3
	},

	make_stone_door = {
		name = "Stone Door",
		workshop = "mason",
		skill = "masonry",
		inputs = {{ item = "stone_boulder", qty = 1 }},
		outputs = {{ item = "door", qty = 1}},
		difficulty = 9,
		time = 10
	},

	make_stone_statue = {
		name = "Stone Statue",
		workshop = "mason",
		skill = "masonry",
		inputs = {{ item = "stone_boulder", qty = 1 }},
		outputs = {{ item = "statue", qty = 1}},
		difficulty = 14,
		time = 15
	},

	make_stone_bed = {
		name = "Stone Bed",
		workshop = "mason",
		skill = "masonry",
		inputs = {{ item = "stone_boulder", qty = 1 }},
		outputs = {{ item = "bed", qty = 1}},
		difficulty = 10,
		time = 10		
	},

	make_stone_chair = {
		name = "Stone Chair",
		workshop = "mason",
		skill = "masonry",
		inputs = {{ item = "stone_boulder", qty = 1 }},
		outputs = {{ item = "chair", qty = 1}},
		difficulty = 10,
		time = 10	
	},

	make_stone_table = {
		name = "Stone Table",
		workshop = "mason",
		skill = "masonry",
		inputs = {{ item = "stone_boulder", qty = 1 }},
		outputs = {{ item = "table", qty = 1}},
		difficulty = 10,
		time = 10	
	},
};