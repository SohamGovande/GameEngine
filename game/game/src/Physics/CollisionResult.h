#pragma once
#include <glm/vec3.hpp>

struct LineIntersectionResult
{
	bool parallel, coinciding;
	glm::vec2 location;

	static inline LineIntersectionResult parallelLines() { return { true, false, glm::vec3(0,0,0) }; };
	static inline LineIntersectionResult coincidingLines() { return { false, true, glm::vec3(0,0,0) }; };
	static inline LineIntersectionResult intersectedAt(const glm::vec2& at) { return { false, false, at }; };
};

struct CollisionResult
{
	bool intersected;
	glm::vec3 location;

	static inline CollisionResult noCollision()
	{
		return { false, glm::vec3(0,0,0) };
	}

	static inline CollisionResult collidedAt(const glm::vec3& location)
	{
		return { true, location };
	}

	static inline CollisionResult collisionExists()
	{
		return { true, glm::vec3(0,0,0) };
	}
};