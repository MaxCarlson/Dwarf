#include "World.h"

std::size_t base_message_t::type_counter = 1;

template<class Container>
void EnsureCapacity(Container& container, typename Container::size_type index) // Revisit this if performance is an issue here
{
	if (container.size() <= index)
	{
		container.resize(index + 1);
	}
}


static const size_t DEFAULT_ENTITY_POOL_SIZE = 1000;


void World::SystemDeleter::operator()(SystemBase * system) const
{
	system->world = nullptr;
	system->entities.clear();
}

World::World() : World(DEFAULT_ENTITY_POOL_SIZE)
{
	pubsub_holder.resize(10);
}

World::World(std::size_t entityPoolSize) : entityIdPool(entityPoolSize), entityAttributes(entityPoolSize)
{
	pubsub_holder.resize(10);
}

void World::addVariadicSystem(SystemBase & sys)
{
	addSystem(sys, ClassTypeId<SystemBase>::autoTypeId());
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

// First deactivate Entity, then add
// it to the cache of entities to be killed on
// next refresh
void World::killEntity(Entity & entity)
{
	deactivateEntity(entity);

	entityCache.killed.push_back(entity);
}

void World::killEntities(std::vector<Entity>& entities)
{
	for (auto& e : entities)
		killEntity(e);
}

void World::activateEntity(Entity & entity)
{
	entityCache.activated.push_back(entity);
}

void World::deactivateEntity(Entity & entity)
{
	entityCache.deactivated.push_back(entity);
}

bool World::isActivated(const Entity & entity) const
{
	if (isValid(entity))
		return entityAttributes.attributes[entity.getId().index].activated;

	return false;
}

bool World::isValid(const Entity & entity) const
{
	return entityIdPool.isValid(entity.getId());
}

void World::refresh() // Split this into three different functions for activated, deactivated, and killed if performance issue
{
	// Loop through all activated entities since
	// last refresh call
	for (auto& entity : entityCache.activated)
	{
		auto& attribute = entityAttributes.attributes[entity.getId().index];
		attribute.activated = true;

		// Loop through every system in this world
		for (auto& sys : systems)
		{
			auto systemIdx = sys.first;

			// If the Entity has the correct components to match the
			// System Filter and isn't part of the system add it to the system
			if (sys.second->getFilter().doesPassFilter(entityAttributes.entityManager.getComponentTypeList(entity)))
			{
				// If the total systems created is less than system index, or we don't have a system
				// at that index, add a system to attributes and add Entity to system 
				if (attribute.systems.size() <= systemIdx || !attribute.systems[systemIdx])
				{
					sys.second->addEntity(entity);

					EnsureCapacity(attribute.systems, systemIdx);
					attribute.systems[systemIdx] = true;
				}
			}
			// If the entity is already in the system, 
			// but we no longer want it to be remove entity from system
			// and mark in attributes that entity is no longer part of system
			else if (attribute.systems.size() > systemIdx && attribute.systems[systemIdx])
			{
				sys.second->remove(entity);
				attribute.systems[systemIdx] = false;
			}
		}
	}

	// Loop through all Entities
	// deactivated since last call of refresh
	for (auto& entity : entityCache.deactivated)
	{
		auto& attribute = entityAttributes.attributes[entity.getId().index];
		attribute.activated = false;

		// Loop through every system in this world and removes the entity from them
		for (auto& sys : systems)
		{
			auto systemIdx = sys.first;
			if (attribute.systems.size() <= systemIdx) continue;

			if (attribute.systems[systemIdx])
			{
				sys.second->remove(entity);
				attribute.systems[systemIdx] = false;
			}
		}
	}

	// Delete killed Entities
	for (auto& entity : entityCache.killed)
	{
		entityCache.alive.erase(std::remove(entityCache.alive.begin(), entityCache.alive.end(), entity), entityCache.alive.end());

		entityAttributes.entityManager.removeAllComponents(entity);

		entityIdPool.remove(entity.getId());
	}

	// Clear all temp cache
	entityCache.clearTemp();
}

void World::clear()
{
	removeAllSystems();

	entityAttributes.clear();

	entityCache.clear();

	entityIdPool.clear();

	ClassTypeId<SystemBase>::resetTypeIdForVariadics();
}

std::size_t World::getEntityCount() const
{
	return entityCache.alive.size();
}

const World::EntityArray & World::getAllEntities() const
{
	return entityCache.alive;
}

World::EntityArray & World::getAllEntities()
{
	return entityCache.alive;
}

Entity World::getEntity(std::size_t idx)
{
	return Entity{ *this, entityIdPool.get(idx) };
}

void World::checkForResize(std::size_t amountOfEntitiesToAllocate)
{
	auto newSize = getEntityCount() + amountOfEntitiesToAllocate;

	if (newSize > entityIdPool.getSize())
		resize(newSize);
}

void World::resize(std::size_t amount)
{
	entityIdPool.resize(amount);
	entityAttributes.resize(amount);
}

void World::addSystem(SystemBase & system, TypeId systemTypeId)
{
	systems[systemTypeId].reset(&system);

	system.world = this;
	system.initialize();
}

void World::removeSystem(TypeId systemTypeId)
{
	systems.erase(systemTypeId);
}

bool World::doesSystemExist(TypeId systemTypeId) const
{
	return systems.find(systemTypeId) != systems.end();
}
