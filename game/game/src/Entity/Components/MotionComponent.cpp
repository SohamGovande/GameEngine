#include "MotionComponent.h"

MotionComponent::MotionComponent()
	: Component(MOTION),
	motion()
{
}

void MotionComponent::tickSpecialized(Entity& entity, Component* parent)
{
	entity.position += motion;
	motion.x = motion.y = motion.z = 0;
}
