#pragma once
#include <glm/vec3.hpp>
#include "Renderer/MaterialModel.h"
#include "ModelLoader.h"

#include "Components/Component.h"

class Entity
{
private:
	glm::vec3 prevPosition;
	glm::vec3 prevRotation;

	std::vector<unsigned int> entityIDs;
	std::vector<Component*> components;

	RenderableMaterialModel* materialModel;
public:
	glm::vec3 position;
	glm::vec3 rotation;

	float scale;

public:
	Entity();
	Entity(const Entity& other);
	~Entity();

	void tick();

	bool hasEntityID(unsigned int id);
	glm::vec3 interpolatePosition(float partialTicks) const;
	glm::vec3 interpolateRotation(float partialTicks) const;
	inline void moveTo(const glm::vec3& position)
	{
		this->position = position;
		this->prevPosition = position;
	}

	inline const std::vector<unsigned int>& getEntityIDs() const { return entityIDs; }

	inline void setMaterialModel(RenderableMaterialModel* model) { this->materialModel = model; }
	inline RenderableMaterialModel* getMaterialModel() const { return materialModel; }

	inline void addEID(unsigned int id) { entityIDs.push_back(id); }

	void addComponent(Component* component);
	
	inline Component& findChild(ComponentID id) const
	{
		for (Component* component : components)
			if (component->getId() == id)
				return *component;

		std::cout << "Unable to find component with ID " << id << " in entity." << std::endl;
		Component* comp = nullptr;
		return *comp;
	}

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
