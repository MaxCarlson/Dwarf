#include "EntityManager.h"

/*
void Entity::addGroup(Group group) noexcept
{
	groupBitset[group] = true;
	manager.addToGroup(this, group);
}
*/

EntityIdPool::EntityIdPool(std::size_t poolSize) 
	: defaultPoolSize(poolSize), nextId(0), counts(poolSize) {};


Entity::Id EntityIdPool::create()
{
	Entity::Id id;

	if (!freeIdList.empty())
	{
		id = freeIdList.back();
		freeIdList.pop_back();
	}
	else
	{
		id.index = nextId++;

		// 0 represents an invalid counter
		// thus here we validate them by setting our counts array
		// as well as the id given outs counter to 1
		counts[id.index] = id.counter = 1;
	}
}

void EntityIdPool::remove(Entity::Id id)
{
	auto& counter = counts[id.index];
	++counter;
	freeIdList.emplace_back(id.index, counter);
}

Entity::Id EntityIdPool::get(std::size_t index) const
{
	// If requested index is within bounds of id's created so far
	// give the id values back
	if (index < counts.size())
		return Entity::Id{ index, counts[index] };

	// Else return an invalid id with index of index
	else
		return Entity::Id{ index, 0 };
}

// If index is outside bounds of already created id's we
// know it's invalid. If that's not the case and the id's counter
// is equal to value stored here, and counter is valid it's true.
bool EntityIdPool::isValid(Entity::Id id) const
{
	if (id.index >= counts.size())
		return false;
	else
		return (id.counter == counts[id.index] && id.counter > 0);
}

std::size_t EntityIdPool::getSize() const
{
	return counts.size();
}

void EntityIdPool::resize(std::size_t amount)
{
	counts.resize(amount);
}

void EntityIdPool::clear()
{
	counts.clear();
	freeIdList.clear();
	nextId = 0;
}
