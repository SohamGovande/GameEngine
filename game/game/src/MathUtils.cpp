#include "MathUtils.h"

namespace Math 
{
	glm::mat4 createProjectionMatrix(float nearPlane, float farPlane, float fov)
	{
		glm::vec2 size = Toolkit::getSize();

		float aspectRatio = (float)size.x / (float)size.y;
		float yScale = (float)(1.0f / tan(DEG2RAD * fov / 2.0f));
		float xScale = yScale / aspectRatio;
		float frustumLength = farPlane - nearPlane;

		glm::mat4 projectionMatrix = glm::mat4(1.0f);
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
		matrix = glm::rotate(matrix, glm::radians(camera.pitch),		glm::vec3(1, 0, 0));
		matrix = glm::rotate(matrix, glm::radians(camera.yaw),		glm::vec3(0, 1, 0));
		matrix = glm::rotate(matrix, glm::radians(camera.roll),		glm::vec3(0, 0, 1));
		matrix = glm::translate(matrix, -camera.position);
		return matrix;
	}
}