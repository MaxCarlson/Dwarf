#include "EntityFactory.h"

#include "ECS\EntityManager.h"
#include "Engine.h"
#include "ECS\World.h"
#include "Tile.h"

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
Entity EntityFactory::createDwarf(Coordinates co)
{
	Entity dwarf = engine.world.createEntity();

	dwarf.addComponent<RenderComponent>(1, 0xE300, "default");
	dwarf.addComponent<PositionComponent>(co);
	dwarf.addComponent<JobComponent>();

	// Need to set stats here, probably as a variable for 
	// this function
	dwarf.addComponent<LaborStatsComponent>();

	// Movement speed in constructor in tiles per second
	dwarf.addComponent<MovementComponent>(2.0);

	//dwarf.addComponent<HealthComponent>(1000, 1000, 1);


	dwarf.activate();

	return dwarf;
}


