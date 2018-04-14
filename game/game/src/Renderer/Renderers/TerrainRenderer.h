#pragma once

#include <glm/mat4x4.hpp>

#include "Terrain/Terrain.h"
#include "../Camera.h"
#include "../Shader/Shader.h"
#include "../MaterialModel.h"
#include "../Light.h"


class TerrainRenderer
{
private:
	Light& light;
	Shader& shader;
	glm::mat4 projectionMatrix;

private:
	void prepareForRendering(const MaterialModel& material) const;
	void renderInstance(float partialTicks, const Terrain& object, const Camera& camera) const;

public:
	TerrainRenderer(Light& light, Shader& shader);

	void draw(float partialTicks, const Camera& camera, const std::list<Terrain*>& terrains) const;

	inline void loadProjectionMatrix(const glm::mat4& mat) { projectionMatrix = mat; }
};
