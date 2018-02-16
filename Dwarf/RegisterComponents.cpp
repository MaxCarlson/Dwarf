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

// Component Sentients
#include "ECS\Components\Sentients\Stats.h"
#include "ECS\Components\Sentients\Inventory.h"
#include "ECS\Components\Sentients\AiWorkComponent.h"
#include "ECS\Components\Sentients\Needs.h"
#include "ECS\Components\Sentients\Happiness.h"

void ComponentsInit::init()
{
	world.registerComponent<PositionComponent>();
	world.registerComponent<MovementComponent>();
	world.registerComponent<RenderComponent>();
	world.registerComponent<ItemCarried>();
	world.registerComponent<ItemStored>();
	world.registerComponent<Item>();
	world.registerComponent<HealthComponent>();
	world.registerComponent<MiningTag>();
	world.registerComponent<BuilderTag>();
	world.registerComponent<Claimed>();
	world.registerComponent<Building>();
	world.registerComponent<Stockpile>();
	world.registerComponent<Seed>();

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

	// Sentiants
	world.registerComponent<Stats>();
	world.registerComponent<Inventory>();
	world.registerComponent<AiWorkComponent>();
	world.registerComponent<Needs>();
	world.registerComponent<Happiness>();

	// Global components
	world.registerComponent<Calender>();
}
