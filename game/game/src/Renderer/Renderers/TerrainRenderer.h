#pragma once

#include <glm/mat4x4.hpp>

#include "Terrain/Terrain.h"
#include "Renderer/Camera.h"
#include "Renderer/Shader/Shader.h"
#include "Renderer/MaterialModel.h"
#include "Renderer/Light.h"

class MasterRenderer;

class TerrainRenderer
{
private:
	MasterRenderer& masterRenderer;
	const std::vector<Light>& lights;
	Shader& shader;
	glm::mat4 projectionMatrix;

private:
	void prepareForRendering(const Terrain& material) const;
	void renderInstance(float partialTicks, const Terrain& object, const Camera& camera);

public:
	TerrainRenderer(MasterRenderer& masterRenderer, const std::vector<Light>& lights, Shader& shader);

	void draw(float partialTicks, const Camera& camera, const std::list<Terrain*>& terrains);

	inline void loadProjectionMatrix(const glm::mat4& mat) { projectionMatrix = mat; }
};
