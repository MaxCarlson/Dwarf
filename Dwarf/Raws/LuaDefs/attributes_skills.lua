
attributes = 
{
	strength = { name = "Strength", id = 0 },
	agility = { name = "Agility", id = 1 },
	toughness = { name = "Toughness", id = 2 },
	endurance = { name = "Endurance", id = 3 },
	intelligence = { name = "Intelligence", id = 4 },
	willpower = { name = "Willpower", id = 5 },
	charisma = { name = "Charisma", id = 6 }, 
};

-- Define skills and what attributes they're based on
-- First attribute is where the success / fail checks will be done,
-- along with the speed multiplier
-- secondary attributes will be minorly improved performing the skill
skills = 
{
	mining = {
		name = "Mining",
		s_attributes = { { attr = "Strength" }, { attr = "Willpower" }, { attr = "Endurance" } }
	},

	masonry = {
		name = "Masonry",
		s_attributes = {{ attr = "Agility" }, { attr = "Endurance" }}
	}, 

	furnace_operation = {
		name = "Furnace Operation",
		s_attributes = {{ attr = "Endurance" }, { attr = "Toughness" }}
	}, 
};
