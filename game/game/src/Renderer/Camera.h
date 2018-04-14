#pragma once
#include "Entity/Entity.h"
#include "GlfwUtils.h"

class World;

class Camera
{
private:
	Entity* controlledEntity;
	glm::ivec2 lastCursorDistance;

	//Stores pitch as x and yaw as y
	glm::vec2 entityAngle;
	glm::vec2 prevEntityAngle;
	float entityDistance;

public:
	float pitch, yaw, roll;
	glm::vec3 position;
	
	void performRotations(float partialTicks);
	void tick(World& world);
	inline void controlEntity(Entity* entity) { controlledEntity = entity; }

	Camera();
	~Camera();
};

