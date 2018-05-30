#pragma once
#include <vector>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer/Camera.h"

#define PI_F 3.141592653f
#define DEG2RAD (PI_F / 180)
#define RAD2DEG (180 / PI_F)

namespace Math {
	
	inline float roundDown(float x, float multiple)
	{
		return (int)(x / multiple - (x < 0 ? ((int)(x / multiple) != multiple ? 1 : 0) : 0)) * multiple;
	}

	inline float cosineInterpolation(float x, float y, float blend)
	{
		float f = (float)(1 - cos(blend * PI_F)) * 0.5f;
		return x * (1.0f - f) + y * f;
	}
	
	inline void clamp(float& x, float low, float high)
	{
		if (x < low)
			x = low;
		if (x > high)
			x = high;
	}
	
	template<typename T>
	inline T interpolate(T src, T dest, float blend)
	{
		return (dest - src) * blend + src;
	}
	
	inline float interpolate(float src, float dest, float blend)
	{
		return (dest - src) * blend + src;
	}

	glm::mat4 createProjectionMatrix(float nearPlane, float farPlane, float fov);
	glm::mat4 createTransformationMatrix(glm::vec3 pos, glm::vec3 rot, float scale);
	glm::mat4 createViewMatrix(const Camera& camera);

	glm::vec3 getBarycentricCoords(const glm::vec2& p, const glm::vec2& a, const glm::vec2& b, const glm::vec2& c);
	float getBarycentricHeight(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec2& pos);
}