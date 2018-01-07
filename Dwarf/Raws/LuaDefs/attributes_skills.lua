
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

	blacksmithing = {
		name = "Blacksmithing",
		s_attributes = {{ attr = "Strength"}, { attr = "Endurance" }}
	},
};
