#include "Entities.h"
#include "World.h"



Entity::Entity() : world(nullptr)
{
}

Entity::Entity(World & entityWorld, Id id) : eId(id), world(&entityWorld)
{
}

bool Entity::isValid() const
{
	if (world == nullptr)
		return false;

	return getWorld().isValid(*this);
}

World & Entity::getWorld() const
{
	return *world;
}

const Entity::Id Entity::getId() const
{
	return eId;
}

bool Entity::isActivated() const
{
	return getWorld().isActivated(*this);
}

void Entity::activate()
{
	getWorld().activateEntity(*this);
}

void Entity::deactivate()
{
	getWorld().deactivateEntity(*this);
}

void Entity::kill()
{
	getWorld().killEntity(*this);
}

void Entity::removeAllComponent()
{
	getWorld().entityAttributes.entityManager.removeAllComponents(*this);
}

ComponentArray Entity::getAllComponents() const
{
	return getWorld().entityAttributes.entityManager.getComponents(*this);
}

ComponentTypeList Entity::getComponentTypeList() const
{
	return getWorld().entityAttributes.entityManager.getComponentTypeList(*this);
}

// "Free" operator overload for comparing to Id's
bool operator==(const Entity::Id& id, const Entity::Id & id1)
{
	return (id.counter == id1.counter && id.index == id1.index);
}

bool Entity::operator==(const Entity &entity) const
{
	return  eId == entity.eId  && entity.world == world;
}

void Entity::addComponent(Component * component, TypeId componentTypeId)
{
	getWorld().entityAttributes.entityManager.addComponent(*this, component, componentTypeId);
}

void Entity::removeComponent(TypeId componentTypeId)
{
	getWorld().entityAttributes.entityManager.removeComponent(*this, componentTypeId);
}

Component& Entity::getComponent(TypeId componentTypeId) const
{
	return getWorld().entityAttributes.entityManager.getComponent(*this, componentTypeId);
}

bool Entity::hasComponent(TypeId componentTypeId) const
{
	return getWorld().entityAttributes.entityManager.hasComponent(*this, componentTypeId);
}
