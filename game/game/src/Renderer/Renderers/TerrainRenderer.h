#pragma once

#include <glm/mat4x4.hpp>

#include "Terrain/Terrain.h"
#include "Renderer/Camera.h"
#include "Renderer/Shader/TerrainShader.h"
#include "Renderer/Shader/Shader.h"
#include "Renderer/MaterialModel.h"
#include "Renderer/Light.h"

class MasterRenderer;

class TerrainRenderer
{
private:
	MasterRenderer& masterRenderer;
	const std::vector<Light>& lights;
	TerrainShader terrainShader;

private:
	void prepareForRendering(const Terrain& material);
	void renderInstance(float partialTicks, const Terrain& object, const Camera& camera);

public:
	TerrainRenderer(MasterRenderer& masterRenderer, const std::vector<Light>& lights, const glm::mat4& projectionMatrix, const std::string& maxLightsStr);
	~TerrainRenderer();

	void render(float partialTicks, const Camera& camera, const std::vector<Terrain*>& terrains);
};
