#pragma once
#include <rapidjson/document.h>
#include <iostream>
#include <vector>
#include <memory>
#include "ComponentIDs.h"

class Entity;

class Component
{
private:
	ComponentID id;

public:
	Component(ComponentID id);

	virtual void setParams(const rapidjson::Value::ConstObject& params);

	inline virtual void tick(Entity& entity, Component* parent) final 
	{
		tickSpecialized(entity, parent);
	}
	
	virtual void tickSpecialized(Entity& entity, Component* parent);

	inline ComponentID getId() const { return id; }
};
