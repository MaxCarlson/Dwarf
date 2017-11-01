#pragma once
#include "Entity.h"
#include "Component.h"

#include <map>



class EntityManager : GameObject
{
public:
	Id generateNewId();
	Entity * createEntity();
	void addComponent(Component * comp, Entity * ent);
	Component * getComponentOfClass();
	void removeEntity(Entity *);

	Id lowestVacantId = 1;
	std::vector<Entity *> entities;
	std::map<CompID, Id> * componentsByClass;
};

