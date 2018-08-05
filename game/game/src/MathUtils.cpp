#include "MathUtils.h"

namespace Math 
{
	glm::mat4 createProjectionMatrix(float nearPlane, float farPlane, float fov)
	{
		glm::uvec2 size = Toolkit::getSize();

		float aspectRatio = (float)size.x / (float)size.y;
		float yScale = (float)(1.0f / tan(DEG2RAD * fov / 2.0f));
		float xScale = yScale / aspectRatio;
		float frustumLength = farPlane - nearPlane;

		glm::mat4 projectionMatrix(1.0f);
		projectionMatrix[0][0] = xScale;
		projectionMatrix[1][1] = yScale;
		projectionMatrix[2][2] = -((farPlane + nearPlane) / frustumLength);
		projectionMatrix[2][3] = -1;
		projectionMatrix[3][2] = -((2 * nearPlane * farPlane) / frustumLength);
		projectionMatrix[3][3] = 0;
		return projectionMatrix;
	}

	glm::mat4 createTransformationMatrix(glm::vec3 translate, glm::vec3 rotate, float scale)
	{
		glm::mat4 matrix = glm::mat4(1.0);

		matrix = glm::translate(matrix, translate);

		matrix = glm::rotate(matrix, glm::radians(rotate.x), glm::vec3(1, 0, 0));
		matrix = glm::rotate(matrix, glm::radians(rotate.y), glm::vec3(0, 1, 0));
		matrix = glm::rotate(matrix, glm::radians(rotate.z), glm::vec3(0, 0, 1));

		matrix = glm::scale(matrix, glm::vec3(scale, scale, scale));

		return matrix;
	}

	//Inverse of transformation matrix
	glm::mat4 createViewMatrix(const Camera& camera)
	{
		glm::mat4 matrix(1.0);
		matrix = glm::rotate(matrix, glm::radians(camera.pitch),	glm::vec3(1, 0, 0));
		matrix = glm::rotate(matrix, glm::radians(camera.yaw),		glm::vec3(0, 1, 0));
		matrix = glm::rotate(matrix, glm::radians(camera.roll),		glm::vec3(0, 0, 1));
		matrix = glm::translate(matrix, -camera.position);
		return matrix;
	}

	glm::vec3 getBarycentricCoords(const glm::vec2& p, const glm::vec2& a, const glm::vec2& b, const glm::vec2& c)
	{
		const glm::vec2 v0 = b - a,
			v1 = c - a,
			v2 = p - a;
		const float d00 = glm::dot(v0, v0);
		const float d01 = glm::dot(v0, v1);
		const float d11 = glm::dot(v1, v1);
		const float d20 = glm::dot(v2, v0);
		const float d21 = glm::dot(v2, v1);
		const float denom = d00 * d11 - d01 * d01;
		glm::vec3 barycentric = glm::vec3(
			(d11 * d20 - d01 * d21) / denom,
			(d00 * d21 - d01 * d20) / denom,
			0
		);
		barycentric.z = 1 - barycentric.x - barycentric.y;
		return barycentric;
	}

	float getBarycentricHeight(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec2& pos) {
		float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
		float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
		float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
		float l3 = 1.0f - l1 - l2;
		return l1 * p1.y + l2 * p2.y + l3 * p3.y;
	}
}