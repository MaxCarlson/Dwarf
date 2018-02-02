#include "EntityFactory.h"

#include "ECS\EntityManager.h"
//#include "Engine.h"
#include "Globals\GlobalWorld.h"
#include "Globals\DwarfContainer.h"
#include "ECS\World.h"

#include "ECS\Components\PositionComponent.h"
#include "ECS\Components\RenderComponent.h"
#include "ECS\Components\HealthComponent.h"
#include "ECS\Components\LaborStatsComponent.h"
#include "ECS\Components\MovementComponent.h"
#include "ECS\Components\JobComponent.h"
#include "ECS\Components\Sentients\Inventory.h"
#include "ECS\Components\Sentients\Stats.h"

#include "Raws\SkillReader.h"

EntityFactory::EntityFactory()
{
}

// Need to create ui interface for starting dwarves
// Also should probably store a vector of dwarves for qucik access
Entity createDwarf(DwarfCreationObj dwarfConstruct)
{
	Entity dwarf = world.createEntity();

	dwarf.addComponent<RenderComponent>(vchar{ 769, color_from_name("default"), color_from_name("default") });
	dwarf.addComponent<PositionComponent>(dwarfConstruct.co);
	dwarf.addComponent<JobComponent>();

	// Convert LaborSkill Points into skill levels
	std::vector<int> laborSkillLevels;
	for (auto& skillPoints : dwarfConstruct.skillPoints)
	{
		// Determine labor skill Level
		const int lbsLevel = determine_labor_skill_Lvl(skillPoints);

		laborSkillLevels.push_back(lbsLevel);
	}
	dwarf.addComponent<LaborStatsComponent>(laborSkillLevels, dwarfConstruct.skillPoints); // Delete this

	std::unordered_map<std::string, attribute> attributes;

	for (const auto& att : allAttributes())
	{
		attributes[att] = attribute{};
	}
	std::unordered_map<std::string, skill> skills;

	for (const auto& sk : allSkills())
	{
		skills[sk] = skill{};
	}

	dwarf.addComponent<Stats>(attributes, skills);

	// Movement speed in constructor in tiles per second
	dwarf.addComponent<MovementComponent>(3.5);

	//dwarf.addComponent<HealthComponent>(1000, 1000, 1);

	dwarf.addComponent<Inventory>();

	dwarf.activate();

	dwarfContainer.dwarves.emplace_back(dwarf);

	// Add dwarves to engine vector
	//engine->Dwarves.push_back(dwarf);

	return dwarf;
}


