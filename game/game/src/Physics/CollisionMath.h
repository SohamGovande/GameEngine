#pragma once
#include <array>
#include <glm/vec3.hpp>
#include "CollisionMath.h"
#include "CollisionResult.h"

namespace CollisionDetection
{
	CollisionResult checkTriangleTriangle(bool calculateIntersectionPoint, std::array<glm::vec3, 3> tri1, std::array<glm::vec3, 3> tri2);
}