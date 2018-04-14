#pragma once
#include <glm/vec3.hpp>
#include "../Entity.h"
#include "Component.h"

class MotionComponent
	: public Component

{
public:
	glm::vec3 motion;

	MotionComponent();
	virtual void tickSpecialized(Entity& entity, Component* parent) override;
};

