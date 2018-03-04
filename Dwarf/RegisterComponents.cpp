#include "stdafx.h"
#include "RegisterComponents.h"
#include "Globals\GlobalWorld.h"

// Components
#include "ECS\Components\PositionComponent.h"
#include "ECS\Components\RenderComponent.h"
#include "ECS\Components\ItemStored.h"
#include "ECS\Components\HealthComponent.h"
#include "ECS\Components\Item.h"
#include "ECS\Components\Claimed.h"
#include "ECS\Components\Building.h"
#include "ECS\Components\ItemCarried.h"
#include "ECS\Components\Stockpile.h"
#include "ECS\Components\Calender.h"
#include "ECS\Components\MovementComponent.h"
#include "ECS\Components\Seed.h"
#include "ECS\Components\Quality.h"

// Ai, some things need to be moved around folder wise
#include "ECS\Components\ai\View.h"

// Component Tags
#include "ECS\Components\Tags\MiningTag.h"
#include "ECS\Components\Tags\BuilderTag.h"
#include "ECS\Components\Tags\WorkOrderTag.h"
#include "ECS\Components\Tags\HaulingTag.h"
#include "ECS\Components\Tags\LumberjacTag.h"
#include "ECS\Components\Tags\HarvestTag.h"
#include "ECS\Components\Tags\PlantingTag.h"
#include "ECS\Components\Tags\ArchitectTag.h"
#include "ECS\Components\Tags\FarmClearTag.h"
#include "ECS\Components\Tags\FarmSoilTag.h"
#include "ECS\Components\Tags\SleepTag.h"
#include "ECS\Components\Tags\EatFoodTag.h"
#include "ECS\Components\Tags\JoyTag.h"
#include "ECS\Components\Tags\MilitaryTag.h"
#include "ECS\Components\Tags\PatrolTag.h"


// Component Sentients
#include "ECS\Components\Sentients\Stats.h"
#include "ECS\Components\Sentients\Inventory.h"
#include "ECS\Components\Sentients\AiWorkComponent.h"
#include "ECS\Components\Sentients\Needs.h"
#include "ECS\Components\Sentients\Happiness.h"
#include "ECS\Components\Sentients\Name.h"

// Weapons and fighting
#include "ECS\Components\Fighting\CombatBase.h"
#include "ECS\Components\Fighting\MeleeWeapon.h"
#include "ECS\Components\Fighting\Drafted.h"

// This enables us to reset each of the variadic eachWith functions to it's
// initial state.
// It's needed because with world.clear() all systems are removed from the world.
// If we want to reload the game, the variadic eachWith's used must be reset and the systems 
// added in the same order, avoiding typeId errors (and Entities belonging to systems they shouldn't)
void ComponentsInit::registerOrResetVardiadicEach(bool reset)
{
	bool first = !reset;

	// Build or reset the variadic systems

	eachWith<Requires<Building>, Excludes<Claimed>>([](auto e) {}, reset, first);
	eachWith<Requires<AiWorkComponent>			  >([](auto e) {}, reset, first);
	eachWith<Requires<MilitaryTag>				  >([](auto e) {}, reset, first);

	for (auto& e : world.getAllEntities())
		e.activate();
	
	world.refresh();
}


void ComponentsInit::init()
{
	world.registerComponent<PositionComponent>();
	world.registerComponent<MovementComponent>();
	world.registerComponent<RenderComponent>();
	world.registerComponent<ItemCarried>(); // This isn't really in use
	world.registerComponent<ItemStored>(); // Not implemented
	world.registerComponent<Item>();
	world.registerComponent<HealthComponent>(); 
	world.registerComponent<Claimed>();
	world.registerComponent<Building>();
	world.registerComponent<Stockpile>();
	world.registerComponent<Seed>();
	world.registerComponent<Quality>();

	// Ai
	world.registerComponent<View>();

	// Tags
	world.registerComponent<MiningTag>();
	world.registerComponent<BuilderTag>();
	world.registerComponent<WorkOrderTag>();
	world.registerComponent<HaulingTag>();
	world.registerComponent<ArchitectTag>();
	world.registerComponent<LumberjacTag>();
	world.registerComponent<HarvestTag>();
	world.registerComponent<PlantingTag>();
	world.registerComponent<FarmClearTag>();
	world.registerComponent<FarmSoilTag>();
	world.registerComponent<SleepTag>();
	world.registerComponent<EatFoodTag>();
	world.registerComponent<JoyTag>();
	world.registerComponent<MilitaryTag>();
	world.registerComponent<PatrolTag>();

	// Sentiants
	world.registerComponent<Stats>();
	world.registerComponent<Inventory>();
	world.registerComponent<AiWorkComponent>();
	world.registerComponent<Needs>();
	world.registerComponent<Happiness>();
	world.registerComponent<Name>();

	// Global components
	world.registerComponent<Calender>();

	// Weapons and fighting
	world.registerComponent<CombatBase>();
	world.registerComponent<MeleeWeapon>();
	world.registerComponent<Drafted>();
}
