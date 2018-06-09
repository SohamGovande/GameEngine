#pragma once
#include <string>
#include <unordered_map>
#include "EntityConstructor.h"
#include "ResourceMgr/ResourceMgr.h"

class EntityRegistry
{
private:
	std::unordered_map<std::string, EntityConstructor> entityCreators;

public:
	EntityRegistry(const ComponentRegistry& componentRegistry, ResourceMgr& resourceMgr);
	
	inline void construct(const std::string& name, Entity& entity) const
	{
		entityCreators.find(name)->second.construct(entity);
	}

	inline const EntityConstructor& getConstructor(const std::string& name) const
	{
		return entityCreators.find(name)->second;
	}

	inline EntityConstructor& getConstructor(const std::string& name)
	{
		return entityCreators.find(name)->second;
	}
};
