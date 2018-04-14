#pragma once
#include <glm/vec3.hpp>
#include "Renderer/MaterialModel.h"
#include "ModelLoader.h"

#include "Components/Component.h"

enum EntityID
{
	PLAYER = 0, TREE = 1, FERN = 2
};

class Entity
{
private:
	glm::vec3 prevPosition;
	glm::vec3 prevRotation;

	std::vector<EntityID> entityIDs;
	std::vector<Component*> components;

	MaterialModel* materialModel;
public:
	glm::vec3 position;
	glm::vec3 rotation;

	float scale;

public:
	Entity(MaterialModel* object);
	~Entity();

	void tick();

	bool hasEntityID(EntityID id);
	glm::vec3 interpolatePosition(float partialTicks) const;
	glm::vec3 interpolateRotation(float partialTicks) const;
	inline void moveTo(const glm::vec3& position)
	{
		this->position = position;
		this->prevPosition = position;
	}

	inline const std::vector<EntityID>& getEntityIDs() const { return entityIDs; }

	inline MaterialModel* getMaterialModel() const { return materialModel; }

	inline void addEID(EntityID id) { entityIDs.push_back(id); }

	void addComponent(Component* const component);

	//EDIT THIS METHOD IN COMPONENT.CPP TOO
	inline Component& findChild(ComponentID id) const
	{
		for (Component* component : components)
		{
			if (component->getId() == id)
				return *const_cast<Component*>(component);
		}

		std::cout << "Unable to find component with ID " << id << " in entity." << std::endl;
		Component* comp = nullptr;
		return *comp;
	}

	//EDIT THIS METHOD IN COMPONENT.CPP TOO
	template<typename T>
	inline std::vector<T*> findChildren(ComponentID id) const
	{
		std::vector<T*> result;
		for (Component* component : components)
			if (component->getId() == id)
				result.push_back((T*)&component);
		return result;
	}


};
