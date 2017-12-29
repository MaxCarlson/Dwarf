#include "EntityManager.h"



EntityIdPool::EntityIdPool(std::size_t poolSize) 
	: defaultPoolSize(poolSize), nextId(1), counts(poolSize) {};


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
	return id;
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
	if (index && index < counts.size())
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
	if (!id.index || id.index >= counts.size())
		return false;
	else
		return (id.index && id.counter == counts[id.index] && id.counter > 0);
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
	nextId = 1;
}

// Entity Manager starts here


EntityManager::EntityManager(std::size_t entityAmount) : componentEntries(entityAmount)
{
}

void EntityManager::addComponent(Entity & entity, Component * component, TypeId componentTypeId)
{
	auto index = entity.getId().index;
	auto& componentData = componentEntries[index];

	componentData.components[componentTypeId].reset(component);
	componentData.componentTypeList[componentTypeId] = true;
}

void EntityManager::removeComponent(Entity & entity, TypeId componentTypeId)
{
	auto index = entity.getId().index;
	auto& componentData = componentEntries[index];

	componentData.components[componentTypeId].reset();
	componentData.componentTypeList[componentTypeId] = false;
}

void EntityManager::removeAllComponents(Entity & entity)
{
	auto index = entity.getId().index;
	auto& componentData = componentEntries[index];

	for (auto& co : componentData.components) co.reset();
	componentData.componentTypeList.reset();
}

Component & EntityManager::getComponent(const Entity & entity, TypeId componentTypeId) const
{
	return *getComponentsArrayImpl(entity)[componentTypeId];
}

ComponentTypeList EntityManager::getComponentTypeList(const Entity & entity) const
{
	return componentEntries[entity.getId().index].componentTypeList;
}

ComponentArray EntityManager::getComponents(const Entity & entity) const
{
	auto& componentsToConvert = getComponentsArrayImpl(entity);

	ComponentArray tmp;
	tmp.reserve(componentsToConvert.size());

	for (auto& i : componentsToConvert)
		tmp.emplace_back(i.get());

	return tmp;
}

bool EntityManager::hasComponent(const Entity & entity, TypeId componentTypeId) const
{
	auto& componentsTmp = getComponentsArrayImpl(entity);

	return (componentsTmp.size() > componentTypeId && componentsTmp[componentTypeId] != nullptr);
}

void EntityManager::resize(std::size_t entityAmount)
{
	componentEntries.resize(entityAmount);
}

void EntityManager::clear()
{
	componentEntries.clear();
}

EntityManager::ImplComponentArray & EntityManager::getComponentsArrayImpl(const Entity & en)
{
	return componentEntries[en.getId().index].components;
}

const EntityManager::ImplComponentArray & EntityManager::getComponentsArrayImpl(const Entity & en) const
{
	return componentEntries[en.getId().index].components;
}
