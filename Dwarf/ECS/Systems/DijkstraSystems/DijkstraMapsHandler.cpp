#include "DijkstraMapsHandler.h"
#include "PickMapSystem.h"
#include "../../Messages/pick_map_changed_message.h"
#include "../../World.h"
#include "../../../Engine.h"

DijkstraMap pick_map;

DijkstraMapsHandler::~DijkstraMapsHandler()
{
	delete pickMapSystem;
}

void DijkstraMapsHandler::init()
{
	// Create systems
	pickMapSystem = new PickMapSystem;

	// Init maps
	pick_map.init();

	// Add systems to world
	engine->world.addSystem(*pickMapSystem);

	// Subscribe to messages
	subscribe_mbox<pick_map_changed_message>();
//	emit(pick_map_changed_message{});
}

void DijkstraMapsHandler::update()
{
	each_mbox<pick_map_changed_message>([this](const pick_map_changed_message &msg) { update_pick_map = true; });

	if (update_pick_map)
	{
		pickMapSystem->update();
		update_pick_map = false;
	}
}
