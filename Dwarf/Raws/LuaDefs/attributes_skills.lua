
-- Add into lua defs phrases to describe skills based on what level the entity is.
-- possibly add it into skills themselves and make a struct to hold names = above

attributes = 
{
	strength = { name = "Strength" },
	agility = { name = "Agility" },
	toughness = { name = "Toughness" },
	endurance = { name = "Endurance" },
	intelligence = { name = "Intelligence" },
	willpower = { name = "Willpower" },
	charisma = { name = "Charisma" }, 
};

-- Define skills and what attributes they're based on
-- First attribute + the skill is where the success / fail checks will be done,
-- the skill level itself affects the speed multiplier
-- secondary attributes will be minorly improved by performing the skill
-- less improvment for each attribute farther from the first
skills = 
{
	-- Fighting skills
	-- TODO: Armor + shield skills

	melee = {
		name = "Melee",
		s_attributes = {{ attr = "Strength"}, { attr = "Agility" }}
	},

	ranged = {
		name = "Ranged",
		s_attributes = {{ attr = "Agility" }, { attr = "Intelligence"}} 
	}, 

	shield = {
		name = "Shield", -- TODO: Get rid of attributes ? -- Add in different skill clasifications e.g. class = "combat" ~ so we can filter them better in gui
	},

	armour = {
		name = "Armour",
	},

	dodging = {
		name = "Dodging",
	},

	-- Job related skills

	mining = {
		name = "Mining",
		s_attributes = { { attr = "Strength" }, { attr = "Willpower" }, { attr = "Endurance" } }
	},

	wood_cutting = {
		name = "Wood Cutting",
		s_attributes = { { attr = "Endurance"}, { attr = "Toughness" } }
	},

	carpentry = {
		name = "Carpentry",
		s_attributes = { { attr = "Willpower" }, { attr = "Intelligence" } }
	},

	masonry = {
		name = "Masonry",
		s_attributes = {{ attr = "Agility" }, { attr = "Endurance" }}
	}, 

	construction = {
		name = "Construction",
		s_attributes = {{ attr = "Strength" }, { attr = "Agility" } }
	},

	furnace_operation = {
		name = "Furnace Operation",
		s_attributes = {{ attr = "Endurance" }, { attr = "Toughness" }}
	}, 

	blacksmithing = {
		name = "Blacksmithing",
		s_attributes = {{ attr = "Strength"}, { attr = "Endurance" }}
	},

	farming = {
		name = "Farming",
		s_attributes = {{ attr = "Intelligence"}, { attr = "Endurance"}}
	},
};
