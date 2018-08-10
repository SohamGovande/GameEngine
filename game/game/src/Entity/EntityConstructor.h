#pragma once
#include <rapidjson/document.h>
#include "Entity.h"
#include "ComponentConstructor.h"
#include "ComponentRegistry.h"
#include "ResourceMgr/ModelResource.h"
#include "ResourceMgr/ResourceMgr.h"

class EntityConstructor
{
private:
	ModelResource* model;
	std::vector<unsigned int> ids;
	std::vector<ComponentConstructor> components;
	const ComponentRegistry& componentRegistry;

public:
	EntityConstructor(const rapidjson::Document& doc, const ComponentRegistry& registry, ResourceMgr& resourceMgr);
	EntityConstructor(EntityConstructor&& other);
	EntityConstructor(const EntityConstructor&& other);

	void construct(Entity& owner) const;
};
