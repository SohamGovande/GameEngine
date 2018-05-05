#include <iostream>
#include "Entity/Components/MotionComponent.h"
#include "Terrain/World.h"
#include "MathUtils.h"
#include "Camera.h"

Camera::Camera()
	: controlledEntity(nullptr),
	lastCursorDistance(),
	entityAngle(0, 0),
	prevEntityAngle(0, 0),
	entityDistance(40),

	pitch(0), yaw(0), roll(0),
	position(0, 0, 0)
{

}

Camera::~Camera()
{
}

void Camera::tick(World& world)
{
	prevEntityAngle = entityAngle;
	Entity& target = *controlledEntity;

	using Keys = sf::Keyboard::Key;

	{
		constexpr float speed = 2;

		float forwardFactor = 0;
		if (Toolkit::isKeyPressed(Keys::W))
			forwardFactor += speed;
		if (Toolkit::isKeyPressed(Keys::S))
			forwardFactor -= speed;

		glm::vec2 direction(
			forwardFactor * sin((target.rotation.y + 90) * DEG2RAD),
			forwardFactor * cos((target.rotation.y + 90) * DEG2RAD)
		);

		glm::vec2 nextPos(
			direction.x + target.position.x,
			direction.y + target.position.z
		);

		float terrainDiff = world.getTerrainHeight(nextPos.x, nextPos.y) - world.getTerrainHeight(target.position.x, target.position.z);
		terrainDiff = 1 / terrainDiff;
		
		if (terrainDiff < 0)
			terrainDiff *= -1.5;

		Math::clamp(terrainDiff, 0.1f, 1);

		if (forwardFactor != 0)
		{
			MotionComponent& motionComp = (MotionComponent&)target.findChild(MOTION);
			motionComp.motion.x = direction.x * terrainDiff;
			motionComp.motion.z = direction.y * terrainDiff;
		}
	}
	{
		int turnFactor = 0;
		constexpr int turnSpeed = 5;

		if (Toolkit::isKeyPressed(Keys::A))
			turnFactor += turnSpeed;

		if (Toolkit::isKeyPressed(Keys::D))
			turnFactor -= turnSpeed;

		if (turnFactor != 0)
		{
			target.rotation.y += turnFactor;
			entityAngle.y += turnFactor;
		}
	}
}

void Camera::performRotations(float partialTicks)
{
	if (!Toolkit::isCursorGrabbed())
		return;

	glm::ivec2 center = Toolkit::getPosition() + glm::ivec2(Toolkit::getSize() / 2u);
	Entity& target = *controlledEntity;
	glm::ivec2 cursorPos = Toolkit::getCursorPos();
	glm::ivec2 distance = cursorPos - center;
	
	Toolkit::setCursorPos(center);

	float sensitivity = .3f;

	entityAngle.y -= distance.x * partialTicks * sensitivity; //yaw
	entityAngle.x -= distance.y * partialTicks * sensitivity; //pitch

	Math::clamp(entityAngle.x, -45, 90);

	lastCursorDistance = cursorPos;

	glm::vec2 smoothFacingEntityRot = Math::interpolate(prevEntityAngle, entityAngle, partialTicks);
	glm::vec3 smoothEntityPos = target.interpolatePosition(partialTicks);

	float horizDistance = entityDistance * cos(smoothFacingEntityRot.x * DEG2RAD);
	float vertDistance = entityDistance * sin(smoothFacingEntityRot.x * DEG2RAD);
	
	glm::vec3 positionOffset(
		sin(smoothFacingEntityRot.y * DEG2RAD) * horizDistance,
		vertDistance + 10, //+head offset
		cos(smoothFacingEntityRot.y * DEG2RAD) * horizDistance
	);
	
	position = smoothEntityPos + positionOffset;

	yaw = -smoothFacingEntityRot.y;
	pitch = smoothFacingEntityRot.x;
	roll = 0;
}
