#pragma once
#include <vector>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer/Camera.h"

#define PI_F 3.141592653f
#define DEG2RAD (PI_F / 180.0f)
#define RAD2DEG (180.0f / PI_F)

namespace Math {
	
	float roundDown(float x, float multiple);
	float cosineInterpolation(float x, float y, float blend);
	void clamp(float& x, float low, float high);
	
	template<typename T>
	inline T linearInterpolation(const T& src, const T& dest, float blend) { return (dest - src) * blend + src; }

	glm::mat4 createProjectionMatrix(float nearPlane, float farPlane, float fov);
	glm::mat4 createModelMatrix(const glm::vec3& pos, const glm::vec3& rot, float scale);
	glm::mat4 createViewMatrix(const Camera& camera);

	glm::vec3 getBarycentricCoords(const glm::vec2& p, const glm::vec2& a, const glm::vec2& b, const glm::vec2& c);
	float getBarycentricHeight(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec2& pos);
}