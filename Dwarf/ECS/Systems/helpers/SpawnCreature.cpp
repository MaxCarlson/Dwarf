#include "stdafx.h"
#include "SpawnCreature.h"
#include "ECS\Messages\spawn_creature_message.h"

void SpawnCreature::init()
{
	subscribe<spawn_creature_message>([this](spawn_creature_message &msg)
	{
		spawnCreature(msg);
	});
}

void SpawnCreature::update(const double duration)
{}

void SpawnCreature::spawnCreature(spawn_creature_message &msg)
{

}
