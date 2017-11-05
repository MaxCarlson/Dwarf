#include "World.h"





void World::SystemDeleter::operator()(SystemBase * system) const
{
	system->world = nullptr;
}

World::World()
{
}

World::World(std::size_t entityPoolSize)
{
}

void World::removeAllSystems()
{
}

Entity World::createEntity()
{
	return Entity();
}

EntityArray World::createEntities(std::size_t number)
{
	return EntityArray();
}

void World::killEntity(Entity & entity)
{
}

void World::killEntities(std::vector<Entity>& entities)
{
}

void World::activateEntity(Entity & entity)
{
}

void World::deactivateEntity(Entity & entity)
{
}

bool World::isActivated(Entity & entity) const
{
	return false;
}

bool World::isValid(Entity & entity) const
{
	return false;
}

void World::refresh()
{
}

void World::clear()
{
}

std::size_t World::getEntityCount() const
{
	return std::size_t();
}

const EntityArray & World::getAllEntities() const
{
	// TODO: insert return statement here
}

Entity World::getEntity(std::size_t idx)
{
	return Entity();
}
