#pragma once
#include <unordered_map>
#include <functional>
#include "Components/ComponentIDs.h"
#include "Components/Component.h"

class ComponentRegistry
{
private:
	std::unordered_map<ComponentID, std::function<void*()>> constructorProvider;
	std::unordered_map<std::string, ComponentID> componentNames;

	template<ComponentID ID, typename T>
	void registerComp(const std::string& name);

public:
	ComponentRegistry();

	inline Component* construct(ComponentID id) const
	{
		return reinterpret_cast<Component*>(constructorProvider.find(id)->second());
	}

	inline Component* construct(const std::string& name) const
	{
		return construct(componentNames.find(name)->second);
	}

	inline ComponentID nameToId(const std::string& name) const
	{
		return componentNames.find(name)->second;
	}
};

