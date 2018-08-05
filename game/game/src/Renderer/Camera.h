#pragma once
#include "Renderer/Shader/SyncWithUniform.t.h"
#include "Entity/Entity.h"
#include "Toolkit.h"

class World;
class MasterRenderer;

class Camera
{
private:
	float nearPlane, farPlane, fov;

	Entity* controlledEntity;
	glm::ivec2 lastCursorDistance;

	//Stores pitch as x and yaw as y
	glm::vec2 entityAngle;
	glm::vec2 prevEntityAngle;
	float entityDistance;

	SyncWithUniform<glm::mat4, 4> projectionMatrix;
	SyncWithUniform<glm::mat4, 4> viewMatrix;

public:
	float pitch, yaw, roll;
	glm::vec3 position;
	
	Camera(MasterRenderer& renderer, float nearPlane, float farPlane, float fov);

	void loadProjectionMatrix();
	void loadViewMatrix();

	void tick(World& world);
	void performRotations(float partialTicks);
	inline void controlEntity(Entity* entity) { controlledEntity = entity; }
};

