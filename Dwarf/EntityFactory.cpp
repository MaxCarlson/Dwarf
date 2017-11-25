#include "EntityFactory.h"

#include "ECS\EntityManager.h"
#include "Engine.h"
#include "ECS\World.h"

#include "ECS\Components\PositionComponent.h"
#include "ECS\Components\RenderComponent.h"
#include "ECS\Components\KeyBoardComponent.h"
#include "ECS\Components\CameraComponent.h"
#include "ECS\Components\HealthComponent.h"
#include "ECS\Components\LaborStatsComponent.h"
#include "ECS\Components\MovementComponent.h"
#include "ECS\Components\JobComponent.h"


EntityFactory::EntityFactory()
{
}

// Need to create ui interface for starting dwarves
// Also should probably store a vector of dwarves for qucik access
Entity EntityFactory::createDwarf(DwarfCreationObj dwarfConstruct)
{
	Entity dwarf = engine.world.createEntity();

	dwarf.addComponent<RenderComponent>(1, 0xE300, "default");
	dwarf.addComponent<PositionComponent>(dwarfConstruct.co);
	dwarf.addComponent<JobComponent>();
	

	// Convert LaborSkill Points into skill levels
	std::vector<int> laborSkillLevels;
	for (auto& skillPoints : dwarfConstruct.skillPoints)
	{
		// Determine labor skill Level
		int lbLevel = determineLaborSkillLvl(skillPoints);

		laborSkillLevels.push_back(lbLevel);
	}
	dwarf.addComponent<LaborStatsComponent>(laborSkillLevels, dwarfConstruct.skillPoints);

	// Movement speed in constructor in tiles per second
	dwarf.addComponent<MovementComponent>(3.5);

	//dwarf.addComponent<HealthComponent>(1000, 1000, 1);

	dwarf.activate();

	engine.Dwarves.push_back(dwarf);

	return dwarf;
}


