
body_defs = {

	human_body = { -- att = Attached to or inside part
	
		head  = { qty = 1				},
		skull = { qty = 1, att = "head" },
		brain = { qty = 1, att = "skull"},
		eye   = { qty = 2, att = "head" },
		ear   = { qty = 2, att = "head" },
		nose  = { qty = 1, att = "head" },
		neck  = { qty = 1, att = "head" },

		torso   = { qty =  1				 },
		arm     = { qty =  2, att = "torso" }, 
		hand    = { qty =  2, att = "arm"    },	  
		finger  = { qty = 10, att = "hand"   },	
		heart   = { qty =  1, att = "torso" },
		lung    = { qty =  2, att = "torso" },
		stomach = { qty =  1, att = "torso" },
		liver   = { qty =  1, att = "torso" },
		kidney  = { qty =  2, att = "torso" },

		waist = { qty =  1				  },
		leg   = { qty =  2, att = "waist" },
		foot  = { qty =  2, att = "leg"   },
		toe   = { qty = 10, att = "foot"  },
	},
};

