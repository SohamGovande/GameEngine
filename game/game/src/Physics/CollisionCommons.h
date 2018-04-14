#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>
#include <vector>
#include "CollisionResult.h"

#define PHYSICS_EPSILON 0.0001f

namespace CollisionMath
{
	template<typename T>
	inline bool areEqual(T x, T y)
	{
		return glm::abs(x - y) <= PHYSICS_EPSILON;
	}

	template<typename T>
	inline bool isZero(T x)
	{
		return glm::abs(x) <= PHYSICS_EPSILON;
	}

	inline float getPointInTriangleSign(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
	{
		return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
	}

	bool isPointInTriangle(const glm::vec3& pt, const std::array<glm::vec3, 3>& triangle);

	LineIntersectionResult findLine2DIntersection(const glm::vec2& e1v1, const glm::vec2& e1v2, const glm::vec2& e2v1, const glm::vec2& e2v2);

	glm::vec2 solveSystem(glm::vec3 e1, const glm::vec3& e2);

	glm::vec4 obtainPlaneEquation(const std::array<glm::vec3, 3>& triangle);
	glm::vec3 findPointOnPlane(const glm::vec4& plane, float x, float z);
	glm::vec3 findPointOnIntersectionLine(const glm::vec4& p1, const glm::vec4& p2);

	std::array<float, 2> calculateXAxisIntersection(const std::array<glm::vec3, 3>& triangle);
	std::vector<std::array<unsigned int, 2>> getEdgesWhichCrossXAxis(const std::array<glm::vec3, 3>& triangle);

	void projectTo2DPlane(glm::vec3& outVertex, const glm::vec3& newOrigin, const glm::mat3& basisVectors);
	void unprojectFrom2DPlane(glm::vec3& outVertex2D, const glm::vec3& originUsed, const glm::mat3& basisVectors);
}
