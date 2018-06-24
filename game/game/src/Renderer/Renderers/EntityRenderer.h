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
	Shader shader, normalMappedShader;
	glm::mat4 projectionMatrix;

private:
	void prepareForRendering(GlStateManager& gl, Shader& shader, const MaterialModel& material);
	void renderInstance(float partialTicks, Shader& shader, const Entity& object, const Camera& camera);

public:
	EntityRenderer(const std::vector<Light>& lights, const glm::mat4& projectionMatrix, const std::string& maxLightsStr);
	~EntityRenderer();

	void render(GlStateManager& gl, float partialTicks, const Camera& camera, const std::unordered_map<MaterialModel, std::list<Entity*>>& entities);

	inline void loadProjectionMatrix(const glm::mat4& mat) { projectionMatrix = mat; }
};