#pragma once
#include <glm/mat4x4.hpp>

#include "Renderer/Camera.h"
#include "Renderer/Shader/Shader.h"
#include "Renderer/MaterialModel.h"
#include "Renderer/Light.h"
#include "Renderer/GlStateManager.h"

class EntityRenderer
{
private:
	const std::vector<Light>& lights;
	Shader& shader;
	glm::mat4 projectionMatrix;

private:
	void prepareForRendering(GlStateManager& gl, const MaterialModel& material);
	void renderInstance(float partialTicks, const Entity& object, const Camera& camera) const;

public:
	EntityRenderer(const std::vector<Light>& lights, Shader& shader);

	void render(GlStateManager& gl, float partialTicks, const Camera& camera, const std::unordered_map<MaterialModel, std::list<Entity*>>& entities);

	inline void loadProjectionMatrix(const glm::mat4& mat) { projectionMatrix = mat; }
};