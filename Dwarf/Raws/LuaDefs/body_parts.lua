

-- TODO: add effects like on % damaged, On destoryed, etc
-- TODO: Add major groups (like head area) and add % covered to individual parts
-- TODO: Add insides  somehow?
-- TODO: Add in sizes e.g. if head is hit skull should have greater chance to be hit than eye, and also greater than brain!
	-- Should they be dynamic? That will make definining species a bit of a pain

pEffects = {
	kill = "kill", -- Kill entity if all parts of this type are gone
	damage_mobility = "damage_mobility",				
}				   

-- TODO: Add in criple mobility
-- TODO: Add in criple workspeed
-- TODO: Add in criple sight

body_parts = {

		head = {
			name = "Head",
			description = "",
			health = 35,
			effects = { pEffects["kill"], },
		},

		skull = {
			name = "Skull",
			description = "",
			health = 35,
			effects = { pEffects["kill"], },
		},

		brain = {
			name = "Brain",
			description = "",
			health = 15,
			effects = { pEffects["kill"], },
		},

		eye = {
			name = "Eye",
			description = "",
			health = 10,
		},

		ear = {
			name = "Ear",
			description = "",
			health = 10,
		},

		nose = {
			name = "Nose",
			description = "",
			health = 15,
		},
	
		neck = {
			name = "Neck",
			description = "",
			health = 20,
		},

		torso = {
			name = "Torso",
			description = "",
			health = 55,
			effects = { pEffects["kill"], },
		},
		arm = {
			name = "Arm",
			description = "",
			health = 35,
		},

		hand = {
			name = "Hand",
			description = "",
			health = 15,
		},

		finger = {
			name = "Finger",
			description = "",
			health = 15,
		},

		heart = {
			name = "Heart",
			description = "",
			health = 8,
			effects = { pEffects["kill"], },
		},

		lung = {
			name = "Lung",
			description = "",
			health = 15,
			effects = { pEffects["kill"], },
		},

		stomach = {
			name = "Stomach",
			description = "",
			health = 15,
			effects = { pEffects["kill"], },
		},

		liver = {
			name = "Liver",
			description = "",
			health = 15,
			effects = { pEffects["kill"], },
		},

		kidney = {
			name = "Kidney",
			description = "",
			health = 15,
			effects = { pEffects["kill"], },
		},

		waist = {
			name = "Waist",
			description = "",
			health = 85,
		},
	
		leg = {
			name = "Leg",
			description = "",
			health = 35,
		},

		foot = {
			name = "Foot",
			description = "",
			health = 20,
		},

		toe = {
			name = "Toe",
			description = "",
			health = 6,
		},
};
