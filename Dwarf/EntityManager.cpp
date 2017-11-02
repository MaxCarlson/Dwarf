#include "EntityManager.h"


/*
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
	return 0;
}

Entity * EntityManager::createEntity()
{
	Id eid = generateNewId();
	entities.push_back(new Entity(eid));
	return entities.back();             // Look up back, does this do exactly what i think it does?
}

void EntityManager::addComponent(Component * comp, Entity * ent)
{
	std::map<Id, Component> * components = componentsByClass[comp->compID]; // Lookup components of this class in dictionary

	// If no map of components inside map, 
	// create a new map (Indexing components by their id's)
	if (!components) {
		components = new std::map<Id, Component>;
		componentsByClass[comp->compID] = components;
	}

	//components[ent->eId] = comp;
}
*/
