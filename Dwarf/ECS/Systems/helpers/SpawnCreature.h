#pragma once
#include "ECS\Systems.h"

struct spawn_creature_message;

class SpawnCreature : public System<Requires<>> 
{
public:
	SpawnCreature() = default;

	void init();
	void update(const double duration); // System does not update, get's fed messages

private:

	void spawnCreature(spawn_creature_message& msg);
	//void spawnItem();
};

