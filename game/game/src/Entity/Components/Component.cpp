#include "Component.h"

Component::Component(ComponentID id)
	: id(id)
{}

void Component::setParams(const rapidjson::Value::ConstObject& params)
{
}

void Component::tickSpecialized(Entity& entity, Component* parent)
{
	
}
