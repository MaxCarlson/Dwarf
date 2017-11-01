#include "EntityManager.h"



EntityManager::EntityManager()
{
	entities.resize(UINT32_MAX);
}

Id EntityManager::generateNewId()
{
	if (lowestVacantId < UINT32_MAX)
		return lowestVacantId++;
	else
		for (Id i = 1; i < UINT32_MAX; ++i)
			if (!entities[i])
				return i;

	printf("Entity allocation error! Out of space!!");
}

Entity * EntityManager::createEntity()
{
	Id eid = generateNewId();
	entities.push_back(new Entity(eid));
	return entities.back();             // Look up back, does this do exactly what i think it does?
}

void EntityManager::addComponent(Component * comp, Entity * ent)
{

}
