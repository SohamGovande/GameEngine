#include <iostream>
#include "MathUtils.h"
#include "Entity.h"

Entity::Entity(MaterialModel* object)
	: materialModel(object),
	prevPosition(), prevRotation(),
	entityIDs(),
	components(),
	position(), rotation(), scale(1)
{
}

Entity::~Entity()
{
	for (Component* component : components)
		delete component;
}

void Entity::tick()
{
	prevPosition = position;
	prevRotation = rotation;

	for (Component* component : components)
		component->tick(*this, nullptr);
}

bool Entity::hasEntityID(EntityID id)
{
	for (EntityID& eid : entityIDs)
		if (eid == id)
			return true;
	return false;
}

glm::vec3 Entity::interpolatePosition(float partialTicks) const
{
	return Math::interpolate(prevPosition, position, partialTicks);
}

glm::vec3 Entity::interpolateRotation(float partialTicks) const
{
	return Math::interpolate(prevRotation, rotation, partialTicks);
}

void Entity::addComponent(Component* const component)
{
	components.push_back(component);
}
