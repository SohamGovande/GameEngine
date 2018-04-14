#pragma once
#include <glm/mat4x4.hpp>

#include "../Camera.h"
#include "../Shader/Shader.h"
#include "../MaterialModel.h"
#include "../Light.h"

class EntityRenderer
{
private:
	Light& light;
	Shader& shader;
	glm::mat4 projectionMatrix;
	bool lastCullingState;

private:
	void prepareForRendering(const MaterialModel& material);
	void renderInstance(float partialTicks, const Entity& object, const Camera& camera) const;

public:
	EntityRenderer(Light& light, Shader& shader);

	void draw(float partialTicks, const Camera& camera, const std::unordered_map<MaterialModel, std::list<Entity*>>& entities);

	inline void loadProjectionMatrix(const glm::mat4& mat) { projectionMatrix = mat; }
};