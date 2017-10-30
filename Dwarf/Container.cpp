#include "Container.h"
#include "Actor.h"


Container::Container(int size) : size(size)
{
}

Container::~Container()
{
	inventory.clearAndDelete();
}

bool Container::add(Actor * actor)
{
	if (size > 0 && inventory.size() >= size) // Don't allow objects/actors added over capacity.
		return false;

	inventory.push(actor);
	return true;
}

void Container::remove(Actor * actor)
{
	inventory.remove(actor);
}
