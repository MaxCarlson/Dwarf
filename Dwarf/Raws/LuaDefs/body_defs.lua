
body_defs = {

	human_body = { -- att = Attached to or inside part
	
		head  = { qty = 1},
		skull = { qty = 1, att = "head"},
		brain = { qty = 1, att = "skull"},
		eye   = { qty = 2, att = "head"},
		ear   = { qty = 2, att = "head"},
		nose  = { qty = 1, att = "head"},
		neck  = { qty = 1, att = "head"},

		torso   = { qty = 1 },
		arm     = { qty = 2, att = "toroso"}, -- Two arms per toroso
		hand    = { qty = 1, att = "arm" },	  -- One hand per arm
		finger  = { qty = 5, att = "hand"},	  -- Five fingers per hand
		heart   = { qty = 1, att = "toroso" },
		lung    = { qty = 2, att = "toroso" },
		stomach = { qty = 1, att = "toroso" },
		liver   = { qty = 1, att = "toroso" },
		kidney  = { qty = 2, att = "toroso" },

		waist = { qty = 1 },
		leg   = { qty = 2, att = "waist" },
		foot  = { qty = 1, att = "leg" },
		toe   = { qty = 5, att = "foot" },
	},
};

