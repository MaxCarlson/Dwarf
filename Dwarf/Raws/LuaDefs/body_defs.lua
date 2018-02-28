
body_defs = {

	human_body = { -- att = Attached to or inside part
	
		head  = { qty = 1,				  hit = 9.00 }, -- TODO: Balance
		skull = { qty = 1, att = "head",  hit = 1.00 }, -- TODO: Make part groups so they can be covered by armor
		brain = { qty = 1, att = "skull", hit = 0.01 }, -- TODO: Once armor is in be sure these defs include which armour piece covers which body part
		eye   = { qty = 2, att = "head",  hit = 0.05 }, -- ^^ Possibly do that in the body_parts.lua file itself
		ear   = { qty = 2, att = "head",  hit = 0.05 },
		nose  = { qty = 1, att = "head",  hit = 1.00 },
		neck  = { qty = 1, att = "head",  hit = 4.00 },

		torso   = { qty =  1, 				  hit = 44.00 }, -- TODO: Health should probably be define here instead of inside the body part!
		arm     = { qty =  2, att = "torso",  hit = 15.00 }, 
		hand    = { qty =  2, att = "arm",    hit = 6.00  },	  
		finger  = { qty = 10, att = "hand",   hit = 0.90  },	
		heart   = { qty =  1, att = "torso",  hit = 0.03  },
		lung    = { qty =  2, att = "torso",  hit = 0.05  },
		stomach = { qty =  1, att = "torso",  hit = 0.04  },
		liver   = { qty =  1, att = "torso",  hit = 0.04  },
		kidney  = { qty =  2, att = "torso",  hit = 0.04  },

		waist = { qty =  1,				   hit = 35.0 },
		leg   = { qty =  2, att = "waist", hit = 20.0 },
		foot  = { qty =  2, att = "leg",   hit = 7.00 },
		toe   = { qty = 10, att = "foot",  hit = 0.06 },
	},
};

