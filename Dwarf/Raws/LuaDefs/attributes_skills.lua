
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
-- along with the speed multiplier
-- secondary attributes will be minorly improved performing the skill
-- less improvment for each attribute farther from the first
skills = 
{
	Mining = {
		name = "Mining",
		s_attributes = { { attr = "Strength" }, { attr = "Willpower" }, { attr = "Endurance" } }
	},

	Masonry = {
		name = "Masonry",
		s_attributes = {{ attr = "Agility" }, { attr = "Endurance" }}
	}, 

	Construction = {
		name = "Construction",
		s_attributes = {{ attr = "Strength" }, { attr = "Agility" } }
	},

	Furnace_Operation = {
		name = "Furnace Operation",
		s_attributes = {{ attr = "Endurance" }, { attr = "Toughness" }}
	}, 

	Blacksmithing = {
		name = "Blacksmithing",
		s_attributes = {{ attr = "Strength"}, { attr = "Endurance" }}
	},
};
