#include "EntityFactory.h"

#include "ECS\EntityManager.h"
#include "Engine.h"
#include "ECS\World.h"

#include "ECS\Components\PositionComponent.h"
#include "ECS\Components\RenderComponent.h"
#include "ECS\Components\HealthComponent.h"
#include "ECS\Components\LaborStatsComponent.h"
#include "ECS\Components\MovementComponent.h"
#include "ECS\Components\JobComponent.h"
#include "ECS\Components\Sentients\Inventory.h"
#include "ECS\Components\Sentients\Stats.h"

EntityFactory::EntityFactory()
{
}

// Need to create ui interface for starting dwarves
// Also should probably store a vector of dwarves for qucik access
Entity EntityFactory::createDwarf(DwarfCreationObj dwarfConstruct)
{
	Entity dwarf = engine->world.createEntity();

	dwarf.addComponent<RenderComponent>(769, 3, "default");
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
	std::unordered_map<std::string, skill> skills;

	dwarf.addComponent<Stats>();

	// Movement speed in constructor in tiles per second
	dwarf.addComponent<MovementComponent>(3.5);

	//dwarf.addComponent<HealthComponent>(1000, 1000, 1);

	dwarf.addComponent<Inventory>();

	dwarf.activate();

	// Add dwarves to engine vector
	engine->Dwarves.push_back(dwarf);

	return dwarf;
}


