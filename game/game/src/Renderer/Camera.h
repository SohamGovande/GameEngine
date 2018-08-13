#pragma once
#include "Renderer/Shader/SyncWithUniform.t.h"
#include "Entity/Entity.h"
#include "Toolkit.h"

class World;
class MasterRenderer;
class GBuffer;

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

	SyncWithUniform<glm::mat4, 5> projectionMatrix;
	SyncWithUniform<glm::mat4, 5> viewMatrix;

	float lastPartialTicks;

public:
	float pitch, yaw, roll;
	glm::vec3 position;
	
	Camera(MasterRenderer& renderer, GBuffer& gbuffer, float nearPlane, float farPlane, float fov);

	void loadProjectionMatrix();
	void loadViewMatrix();

	void tick(World& world);
	void performRotations(float partialTicks, float frameDelta);
	inline void controlEntity(Entity* entity) { controlledEntity = entity; }
};

