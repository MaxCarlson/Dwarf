#include "Ai.h"
#include "Actor.h"
#include "Destructible.h"
#include "Engine.h"
#include "Map.h"
#include "Attacker.h"
#include "Gui.h"

#include <iostream>

// How long ai to track enemys after losing LOS
static const int TRACKING_TURNS = 4;

void PlayerAi::update(Actor * owner)
{
	if (owner->destructible && owner->destructible->isDead())
		return;

	int dx = 0, dy = 0;
	switch (engine.lastKey.vk)
	{

	case TCODK_UP:    dy = -1; break;
	case TCODK_DOWN:  dy =  1; break;
	case TCODK_RIGHT: dx =  1; break;
	case TCODK_LEFT:  dx = -1; break;

	default: break;
	}

	if (dx != 0 || dy != 0) {
		engine.gameStatus = Engine::NEW_TURN;
		if (moveOrAttack(owner, owner->x + dx, owner->y + dy))
			engine.map->computeFov();
	}
}

bool PlayerAi::moveOrAttack(Actor * owner, int targetX, int targetY)
{
	if (engine.map->isWall(targetX, targetY))
		return false;

	for (Actor * actor : engine.actors) // Loop through actors and if any match our target x / y attack them if they're qualified to be attacked
	{
		if (actor->destructible && !actor->destructible->isDead()
			&& actor->x == targetX && actor->y == targetY)
		{	
		owner->attacker->attack(owner, actor);
		return false;
		}
	}

	for (Actor * actor : engine.actors)
		if(actor->destructible->isDead() && actor->x == targetX && actor->y == targetY)
			engine.gui->message(TCODColor::lightGrey, "There's a %s here\n", actor->name);

	// Move the player
	owner->x = targetX;
	owner->y = targetY;
	return true;
}

void MonsterAi::update(Actor * owner)
{
	if (owner->destructible && owner->destructible->isDead()) // Make sure our monster is living
		return;

	if (engine.map->isInFov(owner->x, owner->y))
		oppMoveCount = TRACKING_TURNS;

	else
		--oppMoveCount;
	
	if(oppMoveCount > 0)
		moveOrAttack(owner, engine.player->x, engine.player->y); // Move monster toward player if player can see monster
}

void MonsterAi::moveOrAttack(Actor * owner, int targetX, int targetY)
{
	int dx = targetX - owner->x;
	int dy = targetY - owner->y;
	int stepdx = (dx > 0 ? 1 : -1); // Used to traverse around walls
	int stepdy = (dy > 0 ? 1 : -1);

	float distance = sqrtf(dx*dx + dy*dy);

	if (distance >= 2) {
		dx = (int)(round(dx / distance));
		dy = (int)(round(dy / distance));
		if (engine.map->canWalk(owner->x + dx, owner->y + dy)) { // If square toward player is walkable, walk the monster forward.
			owner->x += dx;
			owner->y += dy;
		}
		else if (engine.map->canWalk(owner->x + stepdx, owner->y))
			owner->x += stepdx;
		else if (engine.map->canWalk(owner->x, owner->y + stepdy))
			owner->y += stepdy;
	}
	else if (owner->attacker)							// Within melee range!
		owner->attacker->attack(owner, engine.player);
}
