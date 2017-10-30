#include "Pickable.h"
#include "Actor.h"
#include "Container.h"
#include "Engine.h"

bool Pickable::pick(Actor * beingPicked, Actor * picking)
{
	if (picking->container && picking->container->add(beingPicked)) {// If person picking has a container, and they have space add actor to actor doing the picking
		engine.actors.remove(beingPicked);							 // Remove actor from rendering since it's been picked up
		return true;
	}
	return false;
}

bool Pickable::useConsume(Actor * beUsed, Actor * user)
{
	if (user->container) {
		user->container->remove(beUsed);
		delete beUsed;
		return true;
	}
	return false;
}
