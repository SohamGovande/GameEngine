#include <iostream>
#include "MathUtils.h"
#include "Entity.h"

Entity::Entity()
	: materialModel(nullptr),
	prevPosition(), prevRotation(),
	entityIDs(),
	components(),
	position(), rotation(), scale(1)
{
	
}

Entity::Entity(const Entity& other)
	: prevPosition(other.prevPosition), prevRotation(other.prevRotation), entityIDs(other.entityIDs), materialModel(other.materialModel),
	position(other.position), rotation(other.rotation), scale(other.scale),
	components()
{
	for (const Component* component : other.components)
		components.push_back(new Component(*component));
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

bool Entity::hasEntityID(unsigned int id)
{
	for (unsigned int eid : entityIDs)
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

void Entity::addComponent(Component* component)
{
	components.push_back(component);
}
