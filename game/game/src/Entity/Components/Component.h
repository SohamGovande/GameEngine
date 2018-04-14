#pragma once
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

	//prevent me from accidentally overriding this method
	inline virtual void tick(Entity& entity, Component* parent) final 
	{
		tickSpecialized(entity, parent);
	}
	
	virtual void tickSpecialized(Entity& entity, Component* parent);

	//EDIT THIS METHOD IN ENTITY.CPP TOO
	template<typename T>
	inline T& findChild(ComponentID id) const
	{
		for (std::unique_ptr<Component>& it : children)
			if (it->id == id)
			{
				Component& ref = *it.get();
				return (T&)ref;
			}
		std::cout << "Unable to find component with ID " << id << " in parent " << this->id << std::endl;

		Component* comp = nullptr;
		return (T&)*comp;
	}

	//EDIT THIS METHOD IN ENTITY.CPP TOO
	template<typename T>
	inline std::vector<T*> findChildren(ComponentID id) const
	{
		std::vector<T*> result;
		for (std::unique_ptr<Component>& component : children)
			if (component->id == id)
				result.push_back((T*)it.get());

		return result;
	}


	inline ComponentID getId() const { return id; }
};
