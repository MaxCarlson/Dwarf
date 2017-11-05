#include "World.h"


static const size_t DEFAULT_ENTITY_POOL_SIZE = 1000;


void World::SystemDeleter::operator()(SystemBase * system) const
{
	system->world = nullptr;
	system->entities.clear();
}

World::World() : World(DEFAULT_ENTITY_POOL_SIZE)
{
}

World::World(std::size_t entityPoolSize) : entityIdPool(entityPoolSize), entityAttributes(entityPoolSize)
{
}

void World::removeAllSystems()
{
	systems.clear();
}

Entity World::createEntity()
{
	checkForResize(1);

	entityCache.alive.emplace_back(*this, entityIdPool.create());

	return entityCache.alive.back();
}

std::vector<Entity> World::createEntities(std::size_t number)
{
	std::vector<Entity> tmp;
	tmp.reserve(number);

	checkForResize(number);

	for (decltype(number) i = 0; i < number; ++i)
	{
		Entity e{ *this, entityIdPool.create() };
		entityCache.alive.push_back(e);
		tmp.push_back(e);
	}

	return tmp;
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
